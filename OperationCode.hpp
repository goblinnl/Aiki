#ifndef OPERATIONCODE_H
#define OPERATIONCODE_H

/* Operation Code 
* Defines a wrapper for modifying byte codes at run-time
*/

// Internal
#include "Codes.hpp"
#include "stack.hpp"

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
	bool				isBigEnd();
	const vector<byte>	getBytecode();
	int					length();
	InteropIter			addInterop(IntermediateOperation *interop);
	void				pushTail(InteropIter it);
	void				popTail();
	bool				buildBytecodeFromIntermediates();

	OperationCode* addByte(byte val);
	OperationCode* addDword(void *dword);
	OperationCode* addInt(int val);
	OperationCode* addUint(uint val);

	void replaceByte(int index, byte val);
	void replaceUint(int index, uint val);

private:
	std::list<IntermediateOperation*> interops;
	Stack<InteropIter>	insertTails;

	vector<byte>		bytes;
	bool				bigEnd;
};

#endif // OPERATIONCODE_H