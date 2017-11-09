#include "DataInstance.h"

DataInstance::DataInstance() : mDestroy(false)
{

}

DataInstance::~DataInstance()
{
	if(!mDestroy || mData == NULL || mData->mDestructor == NULL || mPointer == NULL) {
		return;
	}

	mData->mDestructor(mData->GetParser(), mPointer);
	mPointer = NULL;
}

AKFunction DataInstance::GetOperator(const int& rOperator) const
{
	Data* data = mData;

	while(data != nullptr) {
		int index = AK_FUNCTION_TO_INDEX(rOperator);
		if(data->mOverrideOperator[index] != nullptr) {
			return data->mOverrideOperator[index];
		}

		data = data->GetInheritFrom();
	}

	return nullptr;
}

Variable* DataInstance::GetMethod(const MCString& rKey) const
{
	Data* data = mData;
	while(data != NULL) {
		for(int i = 0; i < mData->GetFunctions().Count(); i++) {
			if(mData->GetFunctions().GetKeyByIndex(i) == rKey) {
				Function* func = new Function(data->GetParser());
				func->mCppFunc = mData->GetFunctions().GetValueByIndex(i);
				func->mIsCpp = true;
				func->mName = rKey;
				func->mFilename = "C++";
				func->SetRef(data->GetParser()->GetCurrentFunction(), 0);

				Variable* temp = data->GetParser()->GetGarbageCollector().GetPooledVariable();
				temp->SetType(Variable::FUNCTION);
				temp->SetData(func);

				return temp;
			}
		}

		data = data->GetInheritFrom();
	}
	return nullptr;
}

Variable* DataInstance::GetMember(const MCString& rKey, bool child /*= false*/) const
{

}

Variable* DataInstance::GetIndex(Variable* rObject, Variable* rKey)
{

}

void DataInstance::SetIndex(Variable* rObject, Variable* rKey, Variable* rValue)
{

}
