#ifndef FRAGMENT_H
#define FRAGMENT_H

// External
#include <stdlib.h>

// Internal
#include "../Codes.h"
#include "../Mixer/MCommon.h"
#include "../Mixer/MString.h"


class OperationCode;
class Parser;
class Tokens;

class Fragment {
public:
	virtual void ParseFragment(Tokens *rTokens, Parser *rParser) = 0;
	virtual void ProvideIntermediates(OperationCode *rOpcode, Parser *rParser) = 0;
	virtual MString	GettingString();

protected:
	uint SetVariable(Parser *rParser, MString rName);
	uint GetVariableID(Parser *rParser, MString rName);
	void AllocateVariable(OperationCode *rOperationCode, uint rVariableID);
};
#endif // FRAGMENT_H
