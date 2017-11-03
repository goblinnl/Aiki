#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "parser.h"

#include "../OperationCode.h"

MString Fragment::GettingString() {
	return "";
}


uint Fragment::SetVariable(Parser *rParser, MString rName) {
	return rParser->RegisterVariable(rName);
}

uint Fragment::GetVariableID(Parser *rParser, MString rName) {
	return rParser->GetVariableID(rName);
}

void Fragment::AllocateVariable(OperationCode *rOpcode, uint rVarID) {
	rOpcode->AddInterop(new ByteOperation(OP_ALLOC));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
}