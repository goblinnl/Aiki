#include "IntermediateOper.h"
#include <cstring>

ByteOperation::ByteOperation(byte aVal) {
	bytes = aVal;
}

void ByteOperation::provideBytecode(OperationCode *aOpcode) {
	aOpcode->addByte(bytes);
}

DwordOperation::DwordOperation(void *aDword) {
	dword = malloc(4);

	if (aDword != NULL) {
		memcpy(dword, aDword, 4);
	}
}

DwordOperation::~DwordOperation() {
	free(dword);
}

void PositionReference::addInquirer(PositionInquirer *aInquirer) {
	inquirers.push_back(aInquirer);
}

void DwordOperation::provideBytecode(OperationCode *aOpcode) {
	aOpcode->addDword(dword);
}

void PositionInquirer::insertPosition(OperationCode *aOpcode, uint aFinalValue) {
	aOpcode->replaceUint(replaceIndex, aFinalValue);
}

void PositionReference::provideBytecode(OperationCode *aOpcode) {
	for (int i=0; i<inquirers.size(); i++) {
		inquirers[i]->insertPosition(aOpcode, aOpcode->length());
	}
}

void PositionInquirer::provideBytecode(OperationCode *aOpcode) {
	replaceIndex = aOpcode->length();
	aOpcode->addUint(0);
}