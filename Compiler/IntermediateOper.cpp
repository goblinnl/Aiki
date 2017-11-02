#include "IntermediateOper.h"
#include <cstring>

ByteOperation::ByteOperation(byte aVal) {
	bytes = aVal;
}

void ByteOperation::ProvideBytecode(OperationCode *aOpcode) {
	aOpcode->AddByte(bytes);
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

void PositionReference::AddInquirer(PositionInquirer *aInquirer) {
	inquirers.push_back(aInquirer);
}

void DwordOperation::ProvideBytecode(OperationCode *aOpcode) {
	aOpcode->AddDword(dword);
}

void PositionInquirer::InsertPosition(OperationCode *aOpcode, uint aFinalValue) {
	aOpcode->ReplaceUint(replaceIndex, aFinalValue);
}

void PositionReference::ProvideBytecode(OperationCode *aOpcode) {
	for (unsigned int i = 0; i < inquirers.size(); i++) {
		inquirers[i]->InsertPosition(aOpcode, aOpcode->Length());
	}
}

void PositionInquirer::ProvideBytecode(OperationCode *aOpcode) {
	replaceIndex = aOpcode->Length();
	aOpcode->AddUint(0);
}