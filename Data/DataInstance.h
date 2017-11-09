#ifndef DATA_INSTANCE_H
#define DATA_INSTANCE_H

// Internal
#include "Data.h"
#include "../Global.h"
#include "../Codes.h"
#include "../Parser/Variable.h"
#include "../Mixer/MCString.h"

class DataInstance {
private:
	void* mPointer;
	bool mDestroy;
	Data* mData;

public:
	DataInstance();
	~DataInstance();

	AKFunction GetOperator(const int& rOperator) const;
	Variable* GetMethod(const MCString& rKey) const;
	Variable* GetMember(const MCString& rKey, bool child = false) const;
	Variable* GetIndex(Variable* rObject, Variable* rKey);

	void SetIndex(Variable* rObject, Variable* rKey, Variable* rValue);
};
#endif // DATA_INSTANCE_H