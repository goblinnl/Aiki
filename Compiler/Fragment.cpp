#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "parser.h"

#include "../OperationCode.h"

MCString Fragment::GettingString() {
	return "";
}


uint Fragment::SetVariable(Parser *rParser, MCString rName) {
	return rParser->RegisterVariable(rName);
}

uint Fragment::GetVariableID(Parser *rParser, MCString rName) {
	return rParser->GetVariableID(rName);
}

void Fragment::AllocateVariable(OperationCode *rOpcode, uint rVarID) {
	rOpcode->AddInterop(new ByteOperation(OP_ALLOC));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
}