#include "IntermediateOper.h"
#include <cstring>

ByteOperation::ByteOperation(byte rVal) {
	mBytes = rVal;
}

void ByteOperation::ProvideBytecode(OperationCode *rOpcode) {
	rOpcode->AddByte(mBytes);
}

DwordOperation::DwordOperation(void *rDword) {
	mDword = malloc(4);

	if(rDword != NULL) {
		memcpy(mDword, rDword, 4);
	}
}

DwordOperation::~DwordOperation() {
	free(mDword);
}

void PositionReference::AddInquirer(PositionInquirer *rInquirer) {
	mInquirers.push_back(rInquirer);
}

void DwordOperation::ProvideBytecode(OperationCode *rOpcode) {
	rOpcode->AddDword(mDword);
}

void PositionInquirer::InsertPosition(OperationCode *rOpcode, uint rFinalValue) {
	rOpcode->ReplaceUint(mReplaceIndex, rFinalValue);
}

void PositionReference::ProvideBytecode(OperationCode *rOpcode) {
	for(unsigned int i = 0; i < mInquirers.size(); i++) {
		mInquirers[i]->InsertPosition(rOpcode, rOpcode->Length());
	}
}

void PositionInquirer::ProvideBytecode(OperationCode *rOpcode) {
	mReplaceIndex = rOpcode->Length();
	rOpcode->AddUint(0);
}