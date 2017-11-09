#include "GarbageCollector.h"

GarbageCollector::GarbageCollector() : mPool(nullptr), mBuffer(nullptr), mBufferSize(0),
mPoolSize(0), mPoolCurrent(-1), mFrame(0), 
mNull(0), mBufferSizeLargest(0)
{
	AllocMoreMemory();
	AllocMorePools();
}

GarbageCollector::~GarbageCollector()
{
	if(mBuffer == nullptr) {
		return;
	}
	Destroy();
}

int GarbageCollector::Start()
{
	int clean = 0;
	mFrame = 0;

	int current = 1536;
	do {
		current = 0;
		for(int i = 0; i < mBufferSize; i++) {
			Variable* temp = mBuffer[i];

			if(temp != nullptr && temp->GetReferenceCounter() == 0) {
				current++;

				if(temp->GetType() == Variable::NUMBER) {
					mPoolNumber.Free((double*)temp->GetData());
					temp->SetData(nullptr);
				}

				if(temp->mPoolReference.mID == -1) {
					delete temp;
				}
				else {
					Pool* pool = mPool[temp->mPoolReference.mID];
					temp->Emp

				}
			}
		}
	} while(current > 0);
		

	
}

int GarbageCollector::StepFrame()
{

}

Variable* GarbageCollector::Function(ArgParser* rParser, ArgumentData* rArg)
{

}

void GarbageCollector::Destroy()
{

}

Variable* GarbageCollector::GetPooledVariable() const
{

}

Variable* GarbageCollector::RegisterVariable(Variable* rVar)
{

}

void GarbageCollector::AllocMoreMemory()
{

}

void GarbageCollector::AllocMorePools()
{

}
