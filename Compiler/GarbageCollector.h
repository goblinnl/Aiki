#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

// Internal
#include "../Global.h"
#include "../Pool/Pool.h"
#include "../Pool/PTyped.h"

class GarbageCollector {
private:
	Pool** mPool;
	int mPoolSize;
	int mPoolCurrent;
	int mBufferSizeLargest;
	int mFrame;

	Variable** mBuffer;
	int mBufferSize;
	int mNull;

public:
	PTyped<double> mPoolNumber;
	PTyped<DataArgument> mPoolArgument;

public:
	GarbageCollector();
	~GarbageCollector();

	int Start();
	void StepFrame();

	static Variable* StaticVar(ArgParser* rParser, DataArgument* rArg);

	void Destroy();

	Variable* GetPooledVariable();
	Variable* RegisterVariable(Variable* rVar);

private:
	void AllocMoreMemory();
	void AllocMorePools();
};
#endif // GARBAGE_COLLECTOR_H