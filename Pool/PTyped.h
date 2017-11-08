#ifndef POOL_TYPED_H
#define POOL_TYPED_H

// Internal
#include "../Global.h"
#include "PLinker.h"

template<class T>
class PTyped {
public:
	PLinker* mNextUnusedLink;
	PLinker* mNextUsedLink;
	PLinker mBufferLink[AK_POOL_SIZE];

public:
	PTyped();
	~PTyped();

	T* GetNext();
	void Free(T* rPValue);
};
#endif // POOL_TYPED_H