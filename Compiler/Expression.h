#ifndef EXPRESSION_H
#define EXPRESSION_H

// Internal
#include "fragment.h"
#include "Function.h"

// External
#include <map>

class FunctionCall;

class Expression : public Fragment {
private:
	bool mIsParam;
	bool nCanBeNull;

public:
	Expression(bool rIsFunctionParam = false, bool rCanBeNull = true);
	~Expression();

	MCString	GettingString();
	void ParseFragment(Tokens *rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);

private:
	struct ExpressionTerm {
		ExpressionTerm(FunctionCall *rFunc) {
			func = rFunc;
			token = NULL;
		}

		ExpressionTerm(Token *rToken) {
			token = rToken;
			func = NULL;
		}

		FunctionCall *func;
		Token *token;
	};

	std::list<ExpressionTerm*> mPostfix;
	std::map<ExpressionTerm*, uint> mExpressionVars;

private:
	void BuildPostfix(Tokens *rTokens, Parser *rParser);
	void AllocateVariables(OperationCode *rOpcode, Parser *rParser);
	void HandleFunctionCalls(OperationCode *rOpcode, Parser *rParser);
	void BuildBytecodePostfix(OperationCode *rOpcode, Parser *rParser);
	void AddOperator(OperationCode *rOpcode, Token *rToken);

	int	OperatorPrecedence(Token *rToken);
};
#endif // EXPRESSION_H