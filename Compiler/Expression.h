#ifndef EXPRESSION_H
#define EXPRESSION_H

// Internal
#include "fragment.h"
#include "Function.h"

// External
#include <map>

class FunctionCall;

class Expression : public Fragment {
public:
	Expression(bool isFunctionParam=false, bool canBeNull=true);
	~Expression();
	std::string	GettingString();

	void ParseFragment(Tokens *aTokens, Parser *aParser);
	void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);

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

	void BuildPostfix(Tokens *tokens, Parser *parser);
	int	OperatorPrecedence(Token *token);

	void AllocateVariables(OperationCode *opcode, Parser *parser);
	void HandleFunctionCalls(OperationCode *opcode, Parser *parser);
	void BuildBytecodePostfix(OperationCode *opcode, Parser *parser);

	void AddOperator(OperationCode *opcode, Token *token);
};

#endif // EXPRESSION_H