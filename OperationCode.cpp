#include "OperationCode.h"
#include "Exception.h"
#include "./Compiler/IntermediateOper.h"

OperationCode::OperationCode() {
	mBigEnd = IsBigEnd();	
	mInsertTails.Push(mInterops.end());
}

bool OperationCode::IsBigEnd() {
	union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

const std::vector<byte> OperationCode::GetBytecode() {
	return mBytes;
}

int OperationCode::Length() {
	return mBytes.size();
}

InteropIter OperationCode::AddInterop(IntermediateOperation *rInterop) {
	InteropIter tail = mInsertTails.Peek();
	return mInterops.insert(tail, rInterop);
}

void OperationCode::PushTail(InteropIter rIt) {
	mInsertTails.Push(rIt);
}

void OperationCode::PopTail() {
	if (mInsertTails.Size() <= 1) {
		throw StackUnderflowException("Stack underflow: internal error");
	}

	mInsertTails.Pop();
}

bool OperationCode::BuildBytecodeFromIntermediates() {
	std::list<IntermediateOperation*>::iterator it;

	for (it = mInterops.begin(); it != mInterops.end(); it++) {
		(*it)->ProvideBytecode(this);
	}

	return true;
}

OperationCode* OperationCode::AddByte(byte rValue) {
	mBytes.push_back(rValue);
	return this;
}

OperationCode* OperationCode::AddDword(void *rDword) {
	byte *b = (byte*)rDword;
	uint xx = *(uint*)rDword;

	if (mBigEnd) {
		for (int i = 3; i >= 0; i--) {
			AddByte(b[i]);
		}
	} else {
		for (int i = 0; i < 4; i++) {
			AddByte(b[i]);
		}
	}

	return this;
}

OperationCode* OperationCode::AddInt(int rValue) {
	return AddDword(&rValue);
}

OperationCode* OperationCode::AddUint(uint rValue) {
	return AddDword(&rValue);
}

void OperationCode::ReplaceByte(int rIndex, byte rValue) {
	if(rIndex < 0 || (unsigned int)rIndex >= mBytes.size()) {
		throw std::range_error("Byte replacement index out of range");
	}
	mBytes[rIndex] = rValue;
}

void OperationCode::ReplaceUint(int rIndex, uint rValue) {
	byte *b = (byte*)&rValue;

	if (mBigEnd) {
		for (int i=3; i>=0; i--) {
			ReplaceByte(rIndex + i, b[i]);
		}
	} 
	else {
		for (int i=0; i<4; i++) {
			ReplaceByte(rIndex + i, b[i]);
		}
	}
}