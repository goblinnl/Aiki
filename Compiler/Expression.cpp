#include "Expression.h"
#include "Function.h"
#include "IntermediateOper.h"
#include "../stack.h"
#include "../Exception.h"



Expression::Expression(bool isFunctionParam, bool canBeNull) {
	isParam = isFunctionParam;
	canBeNull = canBeNull;
}

Expression::~Expression() {
	while (postfix.size()) {
		delete postfix.front();
		postfix.pop_front();
	}
}

string Expression::GettingString() {
	string ret;

	list<ExpressionTerm*>::iterator it;
	for (it = postfix.begin(); it != postfix.end(); it++) {
		if ((*it)->token != NULL) {
			ret += (*it)->token->token + " ";
		} else {
			ret += (*it)->func->GetString();
		}
	}

	return ret;
}


void Expression::ParseFragment(Tokens *tokens, Parser *parser) {
	postfix.clear();

	BuildPostfix(tokens, parser);

	if (!canBeNull && !postfix.size()) {
		throw InvalidTokenException("Expected expression");
	}
}

void Expression::BuildPostfix(Tokens *tokens, Parser *parser) {
	Stack<Token*> stack;

	int paranthCnt = 0;
	const Token *next = tokens->CheckNext();
	while (tokens->IsMore() && next->aType != Token::SEMICOLON && next->aType != Token::COMMA) {

			if (next->aType == Token::VARIABLE_INT || next->aType == Token::VARIABLE_FLOAT) {
				postfix.push_back(new ExpressionTerm(tokens->PopNext()));
			} else if (next->aType == Token::VARIABLE_FUNCTION) {
				Token *token = tokens->PopNext();

				// Function call
				if (tokens->CheckNext()->aType == Token::PARANTH_BEG) {
					FunctionCall *func = new FunctionCall(token);
					func->ParseFragment(tokens, parser);
					postfix.push_back(new ExpressionTerm(func));
				} else {
					postfix.push_back(new ExpressionTerm(token));
				}
			} else if (next->aType & Token::OPERATOR) {
				Token *token = tokens->PopNext();

				while (stack.Size() && stack.Peek()->aType != Token::PARANTH_BEG) {
					if (OperatorPrecedence(stack.Peek()) > OperatorPrecedence(token)) {
						postfix.push_back(new ExpressionTerm(stack.Pop()));
					} else {
						break;
					}
				}

				stack.Push(token);
			} else if (next->aType == Token::PARANTH_BEG) {
				paranthCnt++;
				stack.Push(tokens->PopNext());
			} else if (next->aType == Token::PARANTH_END) {
				paranthCnt--;
				if (paranthCnt < 0) break;

				while (stack.Size() && stack.Peek()->aType != Token::PARANTH_BEG) {
					postfix.push_back(new ExpressionTerm(stack.Pop()));
				}

				// Pop the "("
				stack.Pop();

				// Pop the ")"
				tokens->PopNext();
			} else {
				throw InvalidTokenException("Unexpected token: " + next->token);
			}

			next = tokens->CheckNext();
	}

	while (stack.Size()) {
		postfix.push_back(new ExpressionTerm(stack.Pop()));
	}

	if (!isParam) {
		delete tokens->PopExpected(Token::SEMICOLON);
	}
}

int Expression::OperatorPrecedence(Token *token) {
	if ((token->aType & Token::OPERATOR) == 0) {
		return -1;
	}

	string s = token->token;

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


void Expression::ProvideIntermediates(OperationCode *opcode, Parser *parser) {
	AllocateVariables(opcode, parser);
	HandleFunctionCalls(opcode, parser);

	if (expressionVars.size()) {
		BuildBytecodePostfix(opcode, parser);
	}
}


void Expression::AllocateVariables(OperationCode *opcode, Parser* parser) {
	for (list<ExpressionTerm*>::iterator it=postfix.begin(); it!=postfix.end(); it++) {
			Token *token = (*it)->token;
			if (token && (token->aType & Token::OPERATOR) == 0) {
				expressionVars[*it] = SetVariable(parser, "");
			}
	}

	for (map<ExpressionTerm*,uint>::iterator it=expressionVars.begin(); it != expressionVars.end(); it++) {
			if (!it->first->token) continue;

			Token *token = it->first->token;

			AllocateVariable(opcode, it->second);

			if (token->aType == Token::VARIABLE_FUNCTION) {
				// Copy the original variable into our new one
				opcode->AddInterop(new ByteOperation(OP_MOV));

				uint src = GetVariableID(parser, token->token);

				opcode->AddInterop(new DwordOperation(&it->second));
				opcode->AddInterop(new DwordOperation(&src));
			} else {
				// The value to be copied is a numerical value.
				byte operation = 0;
				void *dword = malloc(4);

				if (it->first->token->aType == Token::VARIABLE_INT) {
					operation = OP_MOVI;
					*(int*)dword = atoi(token->token.c_str());
				} else if (token->aType == Token::VARIABLE_FLOAT) {
					operation = OP_MOVF;
					*(float*)dword = (float)atof(token->token.c_str());
				} else {
					throw InvalidTokenException("Invalid token expression of type");
				}

				opcode->AddInterop(new ByteOperation(operation));
				opcode->AddInterop(new DwordOperation(&it->second));
				opcode->AddInterop(new DwordOperation(dword));

				free(dword);
			}
	}
}

void Expression::HandleFunctionCalls(OperationCode *opcode, Parser *parser) {
	for (list<ExpressionTerm*>::iterator it=postfix.begin(); it!=postfix.end(); it++) {
			FunctionCall *fcall = (*it)->func;
			if (fcall) {
				fcall->ProvideIntermediates(opcode, parser);

				if (postfix.size() > 1) {
					uint id = SetVariable(parser, "");

					AllocateVariable(opcode, id);
					opcode->AddInterop(new ByteOperation(OP_POPMOV));
					opcode->AddInterop(new DwordOperation(&id));

					expressionVars[*it] = id;
				}
			}
	}
}

void Expression::BuildBytecodePostfix(OperationCode *opcode, Parser *parser) {
	for (list<ExpressionTerm*>::iterator it = postfix.begin(); it != postfix.end(); it++) {
			ExpressionTerm *term = *it;

			if (expressionVars.count(term) != 0) {
				opcode->AddInterop(new ByteOperation(OP_PUSH));
				opcode->AddInterop(new DwordOperation(&expressionVars[term]));
			} else {
				AddOperator(opcode, term->token);
			}
	}
}


void Expression::AddOperator(OperationCode *opcode, Token *token) {
	if (!token) {
		throw NullPointerException("Token cannot be nil");
	}

	if ((token->aType & Token::OPERATOR) == 0) {
		throw InvalidTokenException("Expected an operator, got: " + token->token);
	}

	byte oper = 0;
	string s = token->token;

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

	opcode->AddInterop(new ByteOperation(oper));
}