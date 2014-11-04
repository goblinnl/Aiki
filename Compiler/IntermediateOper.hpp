#ifndef INTERMEDIATEOPERATION_H
#define INTERMEDIATEOPERATION_H


// Internal
#include "../OperationCode.hpp"

// External
#include <vector>


class IntermediateOperation {
	public:
		virtual void provideBytecode(OperationCode * aOpcode) = 0;
};


class ByteOperation : public IntermediateOperation {
	public:
		ByteOperation(byte aVal);
		virtual void provideBytecode(OperationCode *aOpcode);
	protected:
		byte bytes;
};


class DwordOperation : public IntermediateOperation {
	public:
		DwordOperation(void *aDword);
		~DwordOperation();
		virtual void provideBytecode(OperationCode *aOpcode);
	protected:
		void *dword;
};


class PositionInquirer : public DwordOperation {
	public:
		PositionInquirer() : DwordOperation(NULL) {}
		virtual void	provideBytecode(OperationCode *aOpcode);
		void			insertPosition(OperationCode *aOpcode, uint aFinalValue);

	private:
		int				replaceIndex;
};

class PositionReference : public IntermediateOperation {
public:
	void			provideBytecode(OperationCode *aOpcode);	
	void			addInquirer(PositionInquirer *aInquirer);

protected:
	vector<PositionInquirer*> inquirers;
};

#endif // INTERMEDIATEOPERATION_H