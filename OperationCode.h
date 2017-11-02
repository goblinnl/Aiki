#ifndef OPERATIONCODE_H
#define OPERATIONCODE_H

/* Operation Code
* Defines a wrapper for modifying byte codes at run-time
*/

// Internal
#include "Codes.h"
#include "stack.h"

// External
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>

class IntermediateOperation;

typedef std::list<IntermediateOperation*>::iterator InteropIter;

class OperationCode {
private:
	std::list<IntermediateOperation*> mInterops;
	Stack<InteropIter> mInsertTails;

	std::vector<byte> mBytes;
	bool mBigEnd;

public:
	OperationCode();
	bool IsBigEnd();
	const std::vector<byte>	GetBytecode();
	int Length();
	InteropIter AddInterop(IntermediateOperation *rInterop);
	void PushTail(InteropIter rIt);
	void PopTail();
	bool BuildBytecodeFromIntermediates();

	OperationCode* AddByte(byte rVal);
	OperationCode* AddDword(void *rDword);
	OperationCode* AddInt(int rVal);
	OperationCode* AddUint(uint rVal);

	void ReplaceByte(int rIndex, byte rVal);
	void ReplaceUint(int rIndex, uint rVal);
};
#endif // OPERATIONCODE_H