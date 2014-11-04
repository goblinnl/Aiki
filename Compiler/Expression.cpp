#include "Expression.hpp"
#include "Function.hpp"
#include "IntermediateOper.hpp"
#include "../stack.hpp"
#include "../Exception.hpp"



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

string Expression::gettingString() {
	string ret;

	list<ExpressionTerm*>::iterator it;
	for (it = postfix.begin(); it != postfix.end(); it++) {
		if ((*it)->token != NULL) {
			ret += (*it)->token->token + " ";
		} else {
			ret += (*it)->func->getString();
		}
	}

	return ret;
}


void Expression::parseFragment(Tokens *tokens, Parser *parser) {
	postfix.clear();

	buildPostfix(tokens, parser);

	if (!canBeNull && !postfix.size()) {
		throw InvalidTokenException("Expected expression");
	}
}

void Expression::buildPostfix(Tokens *tokens, Parser *parser) {
	Stack<Token*> stack;

	int paranthCnt = 0;
	const Token *next = tokens->checkNext();
	while (tokens->isMore() && next->aType != Token::SEMICOLON && next->aType != Token::COMMA) {

			if (next->aType == Token::VARIABLE_INT || next->aType == Token::VARIABLE_FLOAT) {
				postfix.push_back(new ExpressionTerm(tokens->popNext()));
			} else if (next->aType == Token::VARIABLE_FUNCTION) {
				Token *token = tokens->popNext();

				// Function call
				if (tokens->checkNext()->aType == Token::PARANTH_BEG) {
					FunctionCall *func = new FunctionCall(token);
					func->parseFragment(tokens, parser);
					postfix.push_back(new ExpressionTerm(func));
				} else {
					postfix.push_back(new ExpressionTerm(token));
				}
			} else if (next->aType & Token::OPERATOR) {
				Token *token = tokens->popNext();

				while (stack.Size() && stack.peek()->aType != Token::PARANTH_BEG) {
					if (operatorPrecedence(stack.peek()) > operatorPrecedence(token)) {
						postfix.push_back(new ExpressionTerm(stack.pop()));
					} else {
						break;
					}
				}

				stack.push(token);
			} else if (next->aType == Token::PARANTH_BEG) {
				paranthCnt++;
				stack.push(tokens->popNext());
			} else if (next->aType == Token::PARANTH_END) {
				paranthCnt--;
				if (paranthCnt < 0) break;

				while (stack.Size() && stack.peek()->aType != Token::PARANTH_BEG) {
					postfix.push_back(new ExpressionTerm(stack.pop()));
				}

				// Pop the "("
				stack.pop();

				// Pop the ")"
				tokens->popNext();
			} else {
				throw InvalidTokenException("Unexpected token: " + next->token);
			}

			next = tokens->checkNext();
	}

	while (stack.Size()) {
		postfix.push_back(new ExpressionTerm(stack.pop()));
	}

	if (!isParam) {
		delete tokens->popExpected(Token::SEMICOLON);
	}
}

int Expression::operatorPrecedence(Token *token) {
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


void Expression::provideIntermediates(OperationCode *opcode, Parser *parser) {
	allocateVariables(opcode, parser);
	handleFunctionCalls(opcode, parser);

	if (expressionVars.size()) {
		buildBytecodePostfix(opcode, parser);
	}
}


void Expression::allocateVariables(OperationCode *opcode, Parser* parser) {
	for (list<ExpressionTerm*>::iterator it=postfix.begin(); it!=postfix.end(); it++) {
			Token *token = (*it)->token;
			if (token && (token->aType & Token::OPERATOR) == 0) {
				expressionVars[*it] = setVariable(parser, "");
			}
	}

	for (map<ExpressionTerm*,uint>::iterator it=expressionVars.begin(); it != expressionVars.end(); it++) {
			if (!it->first->token) continue;

			Token *token = it->first->token;

			allocateVariable(opcode, it->second);

			if (token->aType == Token::VARIABLE_FUNCTION) {
				// Copy the original variable into our new one
				opcode->addInterop(new ByteOperation(OP_MOV));

				uint src = getVariableID(parser, token->token);

				opcode->addInterop(new DwordOperation(&it->second));
				opcode->addInterop(new DwordOperation(&src));
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

				opcode->addInterop(new ByteOperation(operation));
				opcode->addInterop(new DwordOperation(&it->second));
				opcode->addInterop(new DwordOperation(dword));

				free(dword);
			}
	}
}

void Expression::handleFunctionCalls(OperationCode *opcode, Parser *parser) {
	for (list<ExpressionTerm*>::iterator it=postfix.begin(); it!=postfix.end(); it++) {
			FunctionCall *fcall = (*it)->func;
			if (fcall) {
				fcall->provideIntermediates(opcode, parser);

				if (postfix.size() > 1) {
					uint id = setVariable(parser, "");

					allocateVariable(opcode, id);
					opcode->addInterop(new ByteOperation(OP_POPMOV));
					opcode->addInterop(new DwordOperation(&id));

					expressionVars[*it] = id;
				}
			}
	}
}

void Expression::buildBytecodePostfix(OperationCode *opcode, Parser *parser) {
	for (list<ExpressionTerm*>::iterator it = postfix.begin(); it != postfix.end(); it++) {
			ExpressionTerm *term = *it;

			if (expressionVars.count(term) != 0) {
				opcode->addInterop(new ByteOperation(OP_PUSH));
				opcode->addInterop(new DwordOperation(&expressionVars[term]));
			} else {
				addOperator(opcode, term->token);
			}
	}
}


void Expression::addOperator(OperationCode *opcode, Token *token) {
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

	opcode->addInterop(new ByteOperation(oper));
}