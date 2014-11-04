#ifndef FUNCTION_H
#define FUNCTION_H

// Internal
#include "Fragment.hpp"
#include "tokens.hpp"

// External
#include <stdlib.h>


class Expression;
class PositionReference;

class FunctionCall : public Fragment {
public:
	FunctionCall(Token *aFuncToken);
	void parseFragment(Tokens * aTokens, Parser *aParser);
	void provideIntermediates(OperationCode *aOpcode, Parser *aParser);
	std::string			getString();

protected:
	Token::Type		delimter;
	Token			*functionToken;
	std::list<Expression*>	parameters;
	void handleParameters(OperationCode *aOpcode, Parser *aParser);
};

class FunctionDefinition : public Fragment {
public:
	static bool	isFunctionDefinition(Tokens *aTokens);
	FunctionDefinition();
	PositionReference* getPositionReference();

	void parseFragment(Tokens *aTokens, Parser *aParser);
	void provideIntermediates(OperationCode *aOpcode, Parser *aParser);

	uint getID();

protected:
	std::list<Token*> parameter;
	PositionReference *positionReference;
	uint functionID;
};


class FunctionTail : public Fragment {
public:
	FunctionTail();
	void parseFragment(Tokens *aTokens, Parser *aParser);
	void provideIntermediates(OperationCode *aOpcode, Parser *aParser);
	PositionReference*	getPositionReference();

private:
	PositionReference *posRef;
};

class FunctionSignature {
public:
	FunctionSignature(std::string aName, int aParams);
	std::string getName();
	int getParameterCount();

	uint getID();
	void setID(uint aID);

private:
	std::string signatureName;
	int	parameterCount;
	uint functionID;
};

#endif  // FUNCTION_H