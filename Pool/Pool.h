#ifndef POOL_H
#define POOL_H

// Internal
#include "../Global.h"
#include "../Parser/Variable.h"
#include "PReference.h"
#include "PLinker.h"

class Pool {
public:
	PLinker* mNextUnusedLink;
	PLinker* mNextUsedLink;

	Variable mBuffer[AK_POOL_SIZE];
	PLinker mBufferLinker[AK_POOL_SIZE];

public:
	Pool(int rID);
	~Pool();

	Variable* GetNext();
	void Free(Variable* rValue);
};

#endif // POOL_H