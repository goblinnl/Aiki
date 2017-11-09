#ifndef ARGUMENT_DATA
#define ARGUMENT_DATA

// Internal
#include "Variable.h"
#include "../Compiler/ArgParser.h"

class ArgumentData {
public:
	int mCount;
	Variable* mObject;
	ArgParser* mParser;
	Function* mCaller;
};

#endif // ARGUMENT_DATA