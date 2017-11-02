#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "parser.h"

#include "../OperationCode.h"

string Fragment::GettingString() {
	return "";
}


uint Fragment::SetVariable(Parser *aParser, string aName) {
	return aParser->RegisterVariable(aName);
}

uint Fragment::GetVariableID(Parser *aParser, string aName) {
	return aParser->GetVariableID(aName);
}

void Fragment::AllocateVariable(OperationCode *opcode, uint aVarID) {
	opcode->AddInterop(new ByteOperation(OP_ALLOC));
	opcode->AddInterop(new DwordOperation(&aVarID));
}