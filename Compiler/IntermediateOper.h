#ifndef INTERMEDIATEOPERATION_H
#define INTERMEDIATEOPERATION_H

// Internal
#include "../OperationCode.h"

// External
#include <vector>


class IntermediateOperation {
public:
	virtual void ProvideBytecode(OperationCode * rOpcode) = 0;
};

class ByteOperation : public IntermediateOperation {
protected:
	byte mBytes;

public:
	ByteOperation(byte rVal);
	virtual void ProvideBytecode(OperationCode *rOpcode);

};

class DwordOperation : public IntermediateOperation {
protected:
	void *mDword;

public:
	DwordOperation(void *rDword);
	~DwordOperation();
	virtual void ProvideBytecode(OperationCode *rOpcode);
};

class PositionInquirer : public DwordOperation {
private:
	int mReplaceIndex;

public:
	PositionInquirer() : DwordOperation(NULL) {}
	virtual void ProvideBytecode(OperationCode *rOpcode);
	void InsertPosition(OperationCode *rOpcode, uint rFinalValue);
};

class PositionReference : public IntermediateOperation {
protected:
	std::vector<PositionInquirer*> mInquirers;

public:
	void ProvideBytecode(OperationCode *rOpcode);
	void AddInquirer(PositionInquirer *rInquirer);
};
#endif // INTERMEDIATEOPERATION_H