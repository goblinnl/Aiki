#include "Expression.h"
#include "Function.h"
#include "IntermediateOper.h"
#include "../stack.h"
#include "../Exception.h"



Expression::Expression(bool rIsFunctionParam, bool rCanBeNull) {
	mIsParam = rIsFunctionParam;
	nCanBeNull = rCanBeNull;
}

Expression::~Expression() {
	while (mPostfix.size()) {
		delete mPostfix.front();
		mPostfix.pop_front();
	}
}

MCString Expression::GettingString() {
	MCString ret;

	std::list<ExpressionTerm*>::iterator it;
	for (it = mPostfix.begin(); it != mPostfix.end(); it++) {
		if ((*it)->token != NULL) {
			ret += (*it)->token->mToken + " ";
		} else {
			ret += (*it)->func->GetString();
		}
	}

	return ret;
}

void Expression::ParseFragment(Tokens *rTokens, Parser *rParser) {
	mPostfix.clear();

	BuildPostfix(rTokens, rParser);

	if (!nCanBeNull && !mPostfix.size()) {
		throw InvalidTokenException("Expected expression");
	}
}

void Expression::BuildPostfix(Tokens *rTokens, Parser *rParser) {
	Stack<Token*> stack;

	int paranthCnt = 0;
	const Token *next = rTokens->CheckNext();
	while(rTokens->IsMore() && next->mType != Token::SEMICOLON && next->mType != Token::COMMA) {

		if(next->mType == Token::VARIABLE_INT || next->mType == Token::VARIABLE_FLOAT) {
				mPostfix.push_back(new ExpressionTerm(rTokens->PopNext()));
		}
		else if(next->mType == Token::VARIABLE_FUNCTION) {
				Token *token = rTokens->PopNext();

				// Function call
				if(rTokens->CheckNext()->mType == Token::PARANTH_BEG) {
					FunctionCall *func = new FunctionCall(token);
					func->ParseFragment(rTokens, rParser);
					mPostfix.push_back(new ExpressionTerm(func));
				} else {
					mPostfix.push_back(new ExpressionTerm(token));
				}
		}
		else if(next->mType & Token::OPERATOR) {
				Token *token = rTokens->PopNext();

				while(stack.Size() && stack.Peek()->mType != Token::PARANTH_BEG) {
					if (OperatorPrecedence(stack.Peek()) > OperatorPrecedence(token)) {
						mPostfix.push_back(new ExpressionTerm(stack.Pop()));
					} else {
						break;
					}
				}

				stack.Push(token);
		}
		else if(next->mType == Token::PARANTH_BEG) {
				paranthCnt++;
				stack.Push(rTokens->PopNext());
		}
		else if(next->mType == Token::PARANTH_END) {
				paranthCnt--;
				if (paranthCnt < 0) break;

				while(stack.Size() && stack.Peek()->mType != Token::PARANTH_BEG) {
					mPostfix.push_back(new ExpressionTerm(stack.Pop()));
				}

				// Pop the "("
				stack.Pop();

				// Pop the ")"
				rTokens->PopNext();
			} else {
				throw InvalidTokenException("Unexpected token: " + next->mToken);
			}

			next = rTokens->CheckNext();
	}

	while (stack.Size()) {
		mPostfix.push_back(new ExpressionTerm(stack.Pop()));
	}

	if (!mIsParam) {
		delete rTokens->PopExpected(Token::SEMICOLON);
	}
}

int Expression::OperatorPrecedence(Token *rToken) {
	if((rToken->mType & Token::OPERATOR) == 0) {
		return -1;
	}

	MCString s = rToken->mToken;

	if (s == "=") {
		return 11;
	}

	if (s == "*" ||
		s == "/") {
			return 10;
	} 

	if (s == "+" ||
		s == "-") {
			return 9;
	}

	return 0;
}


void Expression::ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) {
	AllocateVariables(rOpcode, rParser);
	HandleFunctionCalls(rOpcode, rParser);

	if (mExpressionVars.size()) {
		BuildBytecodePostfix(rOpcode, rParser);
	}
}

void Expression::AllocateVariables(OperationCode *rOpcode, Parser* rParser) {
	for(std::list<ExpressionTerm*>::iterator it = mPostfix.begin(); it != mPostfix.end(); it++) {
			Token *token = (*it)->token;
			if(token && (token->mType & Token::OPERATOR) == 0) {
				mExpressionVars[*it] = SetVariable(rParser, "");
			}
	}

	for(std::map<ExpressionTerm*, uint>::iterator it = mExpressionVars.begin(); it != mExpressionVars.end(); it++) {
			if (!it->first->token) continue;

			Token *token = it->first->token;

			AllocateVariable(rOpcode, it->second);

			if(token->mType == Token::VARIABLE_FUNCTION) {
				// Copy the original variable into our new one
				rOpcode->AddInterop(new ByteOperation(OP_MOV));

				uint src = GetVariableID(rParser, token->mToken);

				rOpcode->AddInterop(new DwordOperation(&it->second));
				rOpcode->AddInterop(new DwordOperation(&src));
			} else {
				// The value to be copied is a numerical value.
				byte operation = 0;
				void *dword = malloc(4);

				if(it->first->token->mType == Token::VARIABLE_INT) {
					operation = OP_MOVI;
					*(int*)dword = atoi(token->mToken);
				}
				else if(token->mType == Token::VARIABLE_FLOAT) {
					operation = OP_MOVF;
					*(float*)dword = (float)atof(token->mToken);
				} else {
					throw InvalidTokenException("Invalid token expression of type");
				}

				rOpcode->AddInterop(new ByteOperation(operation));
				rOpcode->AddInterop(new DwordOperation(&it->second));
				rOpcode->AddInterop(new DwordOperation(dword));

				free(dword);
			}
	}
}

void Expression::HandleFunctionCalls(OperationCode *rOpcode, Parser *rParser) {
	for(std::list<ExpressionTerm*>::iterator it = mPostfix.begin(); it != mPostfix.end(); it++) {
			FunctionCall *fcall = (*it)->func;
			if (fcall) {
				fcall->ProvideIntermediates(rOpcode, rParser);

				if (mPostfix.size() > 1) {
					uint id = SetVariable(rParser, "");

					AllocateVariable(rOpcode, id);
					rOpcode->AddInterop(new ByteOperation(OP_POPMOV));
					rOpcode->AddInterop(new DwordOperation(&id));

					mExpressionVars[*it] = id;
				}
			}
	}
}

void Expression::BuildBytecodePostfix(OperationCode *rOpcode, Parser *rParser) {
	for(std::list<ExpressionTerm*>::iterator it = mPostfix.begin(); it != mPostfix.end(); it++) {
			ExpressionTerm *term = *it;

			if (mExpressionVars.count(term) != 0) {
				rOpcode->AddInterop(new ByteOperation(OP_PUSH));
				rOpcode->AddInterop(new DwordOperation(&mExpressionVars[term]));
			} else {
				AddOperator(rOpcode, term->token);
			}
	}
}

void Expression::AddOperator(OperationCode *rOpcode, Token *rToken) {
	if(!rToken) {
		throw NullPointerException("Token cannot be nil");
	}

	if((rToken->mType & Token::OPERATOR) == 0) {
		throw InvalidTokenException("Expected an operator, got: " + rToken->mToken);
	}

	byte oper = 0;
	MCString s = rToken->mToken;

	if (s == "+") {
		oper = OP_ADD;
	} else if (s == "-") {
		oper = OP_SUB;
	} else if (s == "*") {
		oper = OP_MUL;
	} else if (s == "/") {
		oper = OP_DIV;
	} else if (s == "%") {
		oper = OP_MOD;
	} else {
		throw NotImplementedException("Operator is not implemented: " + s);
	}

	rOpcode->AddInterop(new ByteOperation(oper));
}