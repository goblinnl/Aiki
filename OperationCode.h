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

typedef list<IntermediateOperation*>::iterator InteropIter;

class OperationCode {
public:
	OperationCode();
	bool				IsBigEnd();
	const vector<byte>	GetBytecode();
	int					Length();
	InteropIter			AddInterop(IntermediateOperation *interop);
	void				PushTail(InteropIter it);
	void				PopTail();
	bool				BuildBytecodeFromIntermediates();

	OperationCode* AddByte(byte val);
	OperationCode* AddDword(void *dword);
	OperationCode* AddInt(int val);
	OperationCode* AddUint(uint val);

	void ReplaceByte(int index, byte val);
	void ReplaceUint(int index, uint val);

private:
	std::list<IntermediateOperation*> interops;
	Stack<InteropIter>	insertTails;

	vector<byte>		bytes;
	bool				bigEnd;
};

#endif // OPERATIONCODE_H