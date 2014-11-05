#ifndef STATEMENT_H
#define STATEMENT_H

// External
#include <stdlib.h>

// Internal
#include "../codes.hpp"
#include "fragment.hpp"

class Expression;
class Tokens;
struct Token;

class Statement : public Fragment {
	protected:
		Statement();

	public:
		static Statement* createStatement(Tokens *aTokens, Parser *aParser);
};

class IfStatement : public Statement {
	public:
		void parseFragment(Tokens *aTokens, Parser *aParser);
		void provideIntermediates(OperationCode *aOpcode, Parser *aParser);

	private:
};

class AssignStatement : public Statement {
public:
	void parseFragment(Tokens *aTokens, Parser *aParser);
	void provideIntermediates(OperationCode *aOpcode, Parser *aParser);

private:
	bool alloc;
	Token *assignee;
	Token *operators;
	Expression *expression;

	void handleOperator(OperationCode *aOpcode, uint varID);
};

class ReturnStatement : public Statement {
	public:
		void parseFragment(Tokens *aTokens, Parser *aParser);
		void provideIntermediates(OperationCode *aOpcode, Parser *aParser);

	private:
		Expression *expression;
};

#endif // STATEMENT_H