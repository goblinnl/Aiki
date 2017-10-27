#include "Fragment.h"
#include "IntermediateOper.h"
#include "tokens.h"
#include "parser.h"

#include "../OperationCode.h"

string Fragment::gettingString() {
	return "";
}


uint Fragment::setVariable(Parser *aParser, string aName) {
	return aParser->registerVariable(aName);
}

uint Fragment::getVariableID(Parser *aParser, string aName) {
	return aParser->getVariableID(aName);
}

void Fragment::allocateVariable(OperationCode *opcode, uint aVarID) {
	opcode->addInterop(new ByteOperation(OP_ALLOC));
	opcode->addInterop(new DwordOperation(&aVarID));
}