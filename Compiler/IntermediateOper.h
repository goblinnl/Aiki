#ifndef INTERMEDIATEOPERATION_H
#define INTERMEDIATEOPERATION_H

// Internal
#include "../OperationCode.h"

// External
#include <vector>


class IntermediateOperation {
	public:
		virtual void ProvideBytecode(OperationCode * aOpcode) = 0;
};


class ByteOperation : public IntermediateOperation {
	public:
		ByteOperation(byte aVal);
		virtual void ProvideBytecode(OperationCode *aOpcode);
	protected:
		byte bytes;
};


class DwordOperation : public IntermediateOperation {
	public:
		DwordOperation(void *aDword);
		~DwordOperation();
		virtual void ProvideBytecode(OperationCode *aOpcode);
	protected:
		void *dword;
};


class PositionInquirer : public DwordOperation {
	public:
		PositionInquirer() : DwordOperation(NULL) {}
		virtual void	ProvideBytecode(OperationCode *aOpcode);
		void			InsertPosition(OperationCode *aOpcode, uint aFinalValue);

	private:
		int				replaceIndex;
};

class PositionReference : public IntermediateOperation {
public:
	void			ProvideBytecode(OperationCode *aOpcode);	
	void			AddInquirer(PositionInquirer *aInquirer);

protected:
	vector<PositionInquirer*> inquirers;
};

#endif // INTERMEDIATEOPERATION_H