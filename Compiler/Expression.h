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
	void ParseFragment(Tokens *rTokens, ArgParser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser);

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
	void BuildPostfix(Tokens *rTokens, ArgParser *rParser);
	void AllocateVariables(OperationCode *rOpcode, ArgParser *rParser);
	void HandleFunctionCalls(OperationCode *rOpcode, ArgParser *rParser);
	void BuildBytecodePostfix(OperationCode *rOpcode, ArgParser *rParser);
	void AddOperator(OperationCode *rOpcode, Token *rToken);

	int	OperatorPrecedence(Token *rToken);
};
#endif // EXPRESSION_H