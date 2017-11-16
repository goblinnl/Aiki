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
					temp->SetUndefined();
					temp->BindToGarbageCollector(false);
					pool->Free(temp);
				}

				mBuffer[i] = nullptr;
				if(i < mNull) {
					mNull = i;
				}
			}

			clean += current;
		}
	} while(current > 0);

	return clean * sizeof(Variable);
}

void GarbageCollector::StepFrame()
{
	if(mFrame < 512) {
		return;
	}

	mFrame = 0;

	for(int i = 0; i < mBufferSizeLargest + 1; i++) {
		Variable* temp = mBuffer[i];

		if(temp != nullptr && temp->GetReferenceCounter() == 0) {
			// Pooling for numbers
			if(temp->GetType() == Variable::NUMBER) {
				mPoolNumber.Free((double*)temp->GetData());
				temp->SetData(nullptr);
			}

			if(temp->mPoolReference.mID == -1) {
				delete temp;
			}
			else {
				Pool* pool = mPool[temp->mPoolReference.mID];
				temp->SetUndefined();
				temp->BindToGarbageCollector(false);
				pool->Free(temp);
			}

			mBuffer[i] = nullptr;
			if(i < mNull) {
				mNull = i;
			}

			if(i == mBufferSizeLargest) {
				while(mBuffer[--mBufferSizeLargest] == nullptr)
				{

				};
			}
		}
	}
}

Variable* GarbageCollector::StaticVar(ArgParser* rParser, DataArgument* rArg)
{
	return gParse::Instance()->ToVar(rParser, rParser->GetGarbageCollector().Start());
}

void GarbageCollector::Destroy()
{
	// TODO: Debug the Destroy Function
	bool done = false;
	do {
		done = true;

		List<Variable*> variables;
		for(int i = 0; i < mBufferSize; i++) {
			Variable* temp = mBuffer[i];

			if(temp != nullptr) {
				done = false;
				variables.Add(temp);
				if(temp->GetReferenceCounter() == 0) {
					mBuffer[i] = nullptr;

					if(temp->mPoolReference.mID == -1) {
						delete temp;
					}
					else {
						Pool* pool = mPool[temp->mPoolReference.mID];
						temp->SetUndefined();
						temp->BindToGarbageCollector(false);
						pool->Free(temp);
					}
				}

				if(temp->GetType() == Variable::TABLE) {
					temp->GetTable()->Clear();
				}

				if(temp->GetType() == Variable::FUNCTION) {
					Function* func = temp->GetFunction();

					for(int i = 0; i < func->mFixedLocalCount; i++) {
						if(func->mFixedLocalVars[i] != nullptr) {
							AK_REFERENCE_INCREASE(func->mFixedLocalVars[i]);
							func->mFixedLocalVars[i] = nullptr;
						}
					}
				}
			}
		}
	} while(!done);

	for(int i = 0; i < mPoolSize; i++) {
		delete mPool[i];
	}

	free(mBuffer);
	mBuffer = nullptr;

	free(mPool);
	mPool = nullptr;

	mNull = 0;
	mFrame = 0;
	mPoolCurrent = 0;
	mPoolSize = 0;
	mBufferSize = 0;
}

Variable* GarbageCollector::GetPooledVariable()
{

	Variable* temp = mPool[mPoolCurrent]->GetNext();
	if(temp != nullptr) {
		return RegisterVariable(temp);
	}

	for(int i = 0; i < mPoolSize; i++) {
		temp = mPool[mPoolCurrent]->GetNext();

		if(temp != nullptr) {
			return RegisterVariable(temp);
		}
	}

	AllocMorePools();
	return RegisterVariable(mPool[mPoolCurrent]->GetNext());
}

Variable* GarbageCollector::RegisterVariable(Variable* rVar)
{
	if(rVar->GetBindToGarbageCollector()) {
		return rVar;
	}

	rVar->BindToGarbageCollector(true);
	mBuffer[mNull] = rVar;

	if(mNull > mBufferSizeLargest) {
		mBufferSizeLargest = mNull;
	}

	bool isNext = false;
	for(int i = mNull + 1; i < mBufferSize; i++) {
		if(mBuffer[i] == nullptr) {
			mNull = i;
			isNext = true;
			break;
			
		}
	}

	if(!isNext) {
		AllocMoreMemory();
		mNull++;
	}

	mFrame++;
	return rVar;
}

void GarbageCollector::AllocMoreMemory()
{
	mBufferSize += AK_CALLSTACK_SIZE;
	Variable** buffer = (Variable**)malloc(sizeof(Variable*)* mBufferSize);

	if(mBuffer != nullptr) {
		memcpy(buffer, mBuffer, sizeof(Variable*) * (mBufferSize - AK_CALLSTACK_SIZE));
		free(mBuffer);
	}
	memset(buffer + (mBufferSize - AK_CALLSTACK_SIZE), 0, sizeof(Variable*) * 1024);

	mBuffer = buffer;
}

void GarbageCollector::AllocMorePools()
{
	mPoolSize++;
	Pool** buffer = (Pool**)malloc(sizeof(Pool*) * mPoolSize);

	if(mPool != nullptr) {
		memcpy(buffer, mPool, sizeof(Pool*) * (mPoolSize - 1));
		free(mPool);
	}

	mPool = buffer;
	mPoolCurrent++;
	mPool[mPoolCurrent] = new Pool(mPoolCurrent);
}