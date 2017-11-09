#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "ArgParser.h"

#include "../OperationCode.h"

MCString Fragment::GettingString() {
	return "";
}


uint Fragment::SetVariable(ArgParser *rParser, MCString rName) {
	return rParser->RegisterVariable(rName);
}

uint Fragment::GetVariableID(ArgParser *rParser, MCString rName) {
	return rParser->GetVariableID(rName);
}

void Fragment::AllocateVariable(OperationCode *rOpcode, uint rVarID) {
	rOpcode->AddInterop(new ByteOperation(OP_ALLOC));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
}