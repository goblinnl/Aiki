#include "Pool.h"

Pool::Pool(int rID)
{
	for(unsigned int i = 0; i < AK_POOL_SIZE; i++) {
		mBuffer[i].mPoolReference.mIndex = i;
		mBuffer[i].mPoolReference.mID = rID;
	}

	// Linker
	for(unsigned int i = 0; i < AK_POOL_SIZE; i++) {
		mBufferLinker[i].mData = &mBuffer[i];
		
		if(i + 1 < AK_POOL_SIZE) {
			mBufferLinker[i].mNextLink = &mBufferLinker[i + 1];
		}
		else {
			mBufferLinker[i].mNextLink = NULL;
		}
	}

	mNextUnusedLink = &mBufferLinker[0];
	mNextUsedLink = NULL;
}

Pool::~Pool()
{

}

Variable* Pool::GetNext()
{
	if(mNextUsedLink == NULL) {
		return NULL;
	}

	PLinker* link = mNextUnusedLink;
	Variable* lVar = (Variable*)link->mData;
	if(lVar == NULL) {
		return NULL;
	}

	mNextUnusedLink = link->mNextLink;
	mNextUsedLink = link;

	link->mData = NULL;
	return lVar;
}

void Pool::Free(Variable* rValue)
{
	if(mNextUsedLink == NULL) {
		return;
	}

	PLinker* temp = mNextUsedLink->mNextLink;
	mNextUsedLink->mNextLink = mNextUnusedLink;
	mNextUnusedLink = mNextUsedLink;
	mNextUsedLink = temp;

	mNextUnusedLink->mData = rValue;
}
