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
	FunctionCall(Token *aFuncToken);
	void ParseFragment(Tokens * aTokens, Parser *aParser);
	void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);
	std::string	GetString();

protected:
	Token::Type		delimter;
	Token			*functionToken;
	std::list<Expression*>	parameters;
	void HandleParameters(OperationCode *aOpcode, Parser *aParser);
};

class FunctionDefinition : public Fragment {

public:
	static bool	IsFunctionDefinition(Tokens *aTokens);
	FunctionDefinition();
	PositionReference* GetPositionReference();

	void ParseFragment(Tokens *aTokens, Parser *aParser);
	void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);

	uint GetID();

protected:
	std::list<Token*> parameter;
	PositionReference *positionReference;
	uint functionID;
};


class FunctionTail : public Fragment {

public:
	FunctionTail();
	void ParseFragment(Tokens *aTokens, Parser *aParser);
	void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser);
	PositionReference*	GetPositionReference();

private:
	PositionReference *posRef;
};

class FunctionSignature {

public:
	FunctionSignature(std::string aName, int aParams);
	std::string GetName();
	int GetParameterCount();

	uint GetID();
	void SetID(uint aID);

private:
	std::string signatureName;
	int	parameterCount;
	uint functionID;
};

#endif  // FUNCTION_H