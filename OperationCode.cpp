#include "OperationCode.h"
#include "Exception.h"
#include "./Compiler/IntermediateOper.h"

OperationCode::OperationCode() {
	bigEnd = IsBigEnd();	
	insertTails.Push(interops.end());
}

bool OperationCode::IsBigEnd() {
	union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

const vector<byte> OperationCode::GetBytecode() {
	return bytes;
}

int OperationCode::Length() {
	return bytes.size();
}

InteropIter OperationCode::AddInterop(IntermediateOperation *aInterop) {
	InteropIter tail = insertTails.Peek();
	return interops.insert(tail, aInterop);
}

void OperationCode::PushTail(InteropIter aIt) {
	insertTails.Push(aIt);
}

void OperationCode::PopTail() {
	if (insertTails.Size() <= 1) {
		throw StackUnderflowException("Stack underflow: internal error");
	}

	insertTails.Pop();
}

bool OperationCode::BuildBytecodeFromIntermediates() {
	list<IntermediateOperation*>::iterator it;

	for (it = interops.begin(); it != interops.end(); it++) {
		(*it)->ProvideBytecode(this);
	}

	return true;
}

OperationCode* OperationCode::AddByte(byte aValue) {
	bytes.push_back(aValue);
	return this;
}

OperationCode* OperationCode::AddDword(void *aDword) {
	byte *b = (byte*)aDword;
	uint xx = *(uint*)aDword;

	if (bigEnd) {
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

OperationCode* OperationCode::AddInt(int aValue) {
	return AddDword(&aValue);
}

OperationCode* OperationCode::AddUint(uint aValue) {
	return AddDword(&aValue);
}

void OperationCode::ReplaceByte(int aIndex, byte aValue) {
	if (aIndex < 0 || (unsigned int)aIndex >= bytes.size()) {
		throw range_error("Byte replacement index out of range");
	}
	bytes[aIndex] = aValue;
}

void OperationCode::ReplaceUint(int aIndex, uint aValue) {
	byte *b = (byte*)&aValue;

	if (bigEnd) {
		for (int i=3; i>=0; i--) {
			ReplaceByte(aIndex + i, b[i]);
		}
	} else {
		for (int i=0; i<4; i++) {
			ReplaceByte(aIndex + i, b[i]);
		}
	}
}