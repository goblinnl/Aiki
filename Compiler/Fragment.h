#ifndef FRAGMENT_H
#define FRAGMENT_H

// External
#include <stdlib.h>

// Internal
#include "../Codes.h"
#include "../Mixer/MCommon.h"
#include "../Mixer/MCString.h"


class OperationCode;
class ArgParser;
class Tokens;

class Fragment {
public:
	virtual void ParseFragment(Tokens *rTokens, ArgParser *rParser) = 0;
	virtual void ProvideIntermediates(OperationCode *rOpcode, ArgParser *rParser) = 0;
	virtual MCString	GettingString();

protected:
	uint SetVariable(ArgParser *rParser, MCString rName);
	uint GetVariableID(ArgParser *rParser, MCString rName);
	void AllocateVariable(OperationCode *rOperationCode, uint rVariableID);
};
#endif // FRAGMENT_H
