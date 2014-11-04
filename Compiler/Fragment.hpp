#ifndef FRAGMENT_H
#define FRAGMENT_H

// External
#include <stdlib.h>
#include <string>

// Internal
#include "../Codes.hpp"

class OperationCode;
class Parser;
class Tokens;

class Fragment {
public:
	virtual void parseFragment(Tokens *aTokens, Parser *aParser) = 0;
	virtual void provideIntermediates(OperationCode *aOpcode, Parser *aParser) = 0;	
	virtual std::string	gettingString();

protected:
	uint setVariable(Parser *aParser, std::string aName);
	uint getVariableID(Parser *aParser, std::string aName);
	void allocateVariable(OperationCode *aOperationCode, uint aVariableID);
};

#endif // FRAGMENT_H
