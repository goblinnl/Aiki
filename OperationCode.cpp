#include "OperationCode.hpp"
#include "Exception.hpp"
#include "./Compiler/IntermediateOper.hpp"

OperationCode::OperationCode() {
	bigEnd = isBigEnd();	
	insertTails.push(interops.end());
}

bool OperationCode::isBigEnd() {
	union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

const vector<byte> OperationCode::getBytecode() {
	return bytes;
}

int OperationCode::length() {
	return bytes.size();
}

InteropIter OperationCode::addInterop(IntermediateOperation *aInterop) {
	InteropIter tail = insertTails.peek();
	return interops.insert(tail, aInterop);
}

void OperationCode::pushTail(InteropIter aIt) {
	insertTails.push(aIt);
}

void OperationCode::popTail() {
	if (insertTails.Size() <= 1) {
		throw StackUnderflowException("Stack underflow: internal error");
	}

	insertTails.pop();
}

bool OperationCode::buildBytecodeFromIntermediates() {
	list<IntermediateOperation*>::iterator it;

	for (it = interops.begin(); it != interops.end(); it++) {
		(*it)->provideBytecode(this);
	}

	return true;
}

OperationCode* OperationCode::addByte(byte aValue) {
	bytes.push_back(aValue);
	return this;
}

OperationCode* OperationCode::addDword(void *aDword) {
	byte *b = (byte*)aDword;
	uint xx = *(uint*)aDword;

	if (bigEnd) {
		for (int i = 3; i >= 0; i--) {
			addByte(b[i]);
		}
	} else {
		for (int i = 0; i < 4; i++) {
			addByte(b[i]);
		}
	}

	return this;
}

OperationCode* OperationCode::addInt(int aValue) {
	return addDword(&aValue);
}

OperationCode* OperationCode::addUint(uint aValue) {
	return addDword(&aValue);
}

void OperationCode::replaceByte(int aIndex, byte aValue) {
	if (aIndex < 0 || aIndex >= bytes.size()) {
		throw range_error("Byte replacement index out of range");
	}
	bytes[aIndex] = aValue;
}

void OperationCode::replaceUint(int aIndex, uint aValue) {
	byte *b = (byte*)&aValue;

	if (bigEnd) {
		for (int i=3; i>=0; i--) {
			replaceByte(aIndex + i, b[i]);
		}
	} else {
		for (int i=0; i<4; i++) {
			replaceByte(aIndex + i, b[i]);
		}
	}
}