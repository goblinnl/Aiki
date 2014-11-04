#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "fragment.hpp"
#include "Function.hpp"

#include <map>

class FunctionCall;

class Expression : public Fragment {
public:
	Expression(bool isFunctionParam=false, bool canBeNull=true);
	~Expression();
	std::string	gettingString();

	void parseFragment(Tokens *aTokens, Parser *aParser);
	void provideIntermediates(OperationCode *aOpcode, Parser *aParser);

private:
	struct ExpressionTerm {
		ExpressionTerm(FunctionCall *aFunc) {
			func = aFunc;
			token = NULL;
		}

		ExpressionTerm(Token *aToken) {
			token = aToken;
			func = NULL;
		}

		FunctionCall *func;
		Token *token;
	};

	std::list<ExpressionTerm*> postfix;
	bool isParam;
	bool canBeNull;

	std::map<ExpressionTerm*,uint> expressionVars;

	void buildPostfix(Tokens *tokens, Parser *parser);
	int	operatorPrecedence(Token *token);

	void allocateVariables(OperationCode *opcode, Parser *parser);
	void handleFunctionCalls(OperationCode *opcode, Parser *parser);
	void buildBytecodePostfix(OperationCode *opcode, Parser *parser);

	void addOperator(OperationCode *opcode, Token *token);
};

#endif // EXPRESSION_H