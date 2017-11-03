#ifndef FUNCTION_H
#define FUNCTION_H

// Internal
#include "Fragment.h"
#include "tokens.h"

// External
#include <stdlib.h>

class Expression;
class PositionReference;

class FunctionCall : public Fragment {
public:
	FunctionCall(Token *rFuncToken);
	void ParseFragment(Tokens * rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);
	MString	GetString();

protected:
	Token::Type		mDelimter;
	Token			*mFunctionToken;
	std::list<Expression*>	mParameters;
	void HandleParameters(OperationCode *rOpcode, Parser *rParser);
};

class FunctionDefinition : public Fragment {
protected:
	std::list<Token*> mParameter;
	PositionReference *mPositionReference;
	uint mFunctionID;

public:
	FunctionDefinition();
	PositionReference* GetPositionReference();

	void ParseFragment(Tokens *aTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);

	uint GetID();

	static bool	IsFunctionDefinition(Tokens *rTokens);

};

class FunctionTail : public Fragment {
public:
	FunctionTail();
	void ParseFragment(Tokens *rTokens, Parser *rParser);
	void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser);
	PositionReference*	GetPositionReference();

private:
	PositionReference *mPosRef;
};

class FunctionSignature {
private:
	MString mSignatureName;
	int	mParameterCount;
	uint mFunctionID;

public:
	FunctionSignature(MString rName, int rParams);
	MString GetName();
	int GetParameterCount();

	uint GetID();
	void SetID(uint rID);
};
#endif  // FUNCTION_H