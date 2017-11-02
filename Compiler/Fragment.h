#ifndef FRAGMENT_H
#define FRAGMENT_H

// External
#include <stdlib.h>
#include <string>

// Internal
#include "../Codes.h"

class OperationCode;
class Parser;
class Tokens;

class Fragment {
public:
	virtual void ParseFragment(Tokens *aTokens, Parser *aParser) = 0;
	virtual void ProvideIntermediates(OperationCode *aOpcode, Parser *aParser) = 0;	
	virtual std::string	GettingString();

protected:
	uint SetVariable(Parser *aParser, std::string aName);
	uint GetVariableID(Parser *aParser, std::string aName);
	void AllocateVariable(OperationCode *aOperationCode, uint aVariableID);
};

#endif // FRAGMENT_H
