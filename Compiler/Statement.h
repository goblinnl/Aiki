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
	static Statement* CreateStatement(Tokens *rTokens, Parser *rParser);
};

class IfStatement : public Statement {
private:
	bool mAlloc;
	Token *mAssignee;
	Token *mOperators;
	Expression *mExpression;

public:
	void ParseFragment(Tokens *rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);
};

class AssignStatement : public Statement {
private:
	bool mAlloc;
	Token *mAssignee;
	Token *mOperators;
	Expression *mExpression;

public:
	void ParseFragment(Tokens *rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);

private:
	void HandleOperator(OperationCode *rOpcode, uint rVarID);
};

class ReturnStatement : public Statement {
private:
	Expression *mExpression;

public:
	void ParseFragment(Tokens *rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);
};
#endif // STATEMENT_H