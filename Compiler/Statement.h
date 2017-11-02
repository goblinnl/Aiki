#ifndef STATEMENT_H
#define STATEMENT_H

// External
#include <stdlib.h>

// Internal
#include "../codes.h"
#include "fragment.h"

class Expression;
class Tokens;
struct Token;

class Statement : public Fragment {
	protected:
		Statement();

	public:
		static Statement* CreateStatement(Tokens *aTokens, Parser *aParser);
};

class IfStatement : public Statement {
	public:
		void ParseFragment(Tokens *aTokens, Parser *aParser);
		void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);

	private:
		bool alloc;
		Token *assignee;
		Token *operators;
		Expression *expression;
};

class AssignStatement : public Statement {
public:
	void ParseFragment(Tokens *aTokens, Parser *aParser);
	void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);

private:
	bool alloc;
	Token *assignee;
	Token *operators;
	Expression *expression;

	void HandleOperator(OperationCode *aOpcode, uint varID);
};

class ReturnStatement : public Statement {
	public:
		void ParseFragment(Tokens *aTokens, Parser *aParser);
		void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);

	private:
		Expression *expression;
};

#endif // STATEMENT_H