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
	virtual void ParseFragment(Tokens *rTokens, Parser *rParser) = 0;
	virtual void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) = 0;
	virtual std::string	GettingString();

protected:
	uint SetVariable(Parser *rParser, std::string rName);
	uint GetVariableID(Parser *rParser, std::string rName);
	void AllocateVariable(OperationCode *rOperationCode, uint rVariableID);
};
#endif // FRAGMENT_H
