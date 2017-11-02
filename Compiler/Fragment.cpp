#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "parser.h"

#include "../OperationCode.h"

std::string Fragment::GettingString() {
	return "";
}


uint Fragment::SetVariable(Parser *rParser, std::string rName) {
	return rParser->RegisterVariable(rName);
}

uint Fragment::GetVariableID(Parser *rParser, std::string rName) {
	return rParser->GetVariableID(rName);
}

void Fragment::AllocateVariable(OperationCode *rOpcode, uint rVarID) {
	rOpcode->AddInterop(new ByteOperation(OP_ALLOC));
	rOpcode->AddInterop(new DwordOperation(&rVarID));
}