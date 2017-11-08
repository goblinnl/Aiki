#include "PTyped.h"

template<class T>
PTyped<T>::PTyped()
{
	for(unsigned int i = 0; i < AK_POOL_SIZE; i++) {
		mBufferLink[i].mData = new T();
		
		if(i + 1 < AK_POOL_SIZE) {
			mBufferLink[i].mNextLink = &mBufferLink[i + 1];
		}
		else {
			mBufferLink[i].mNextLink = NULL;
		}
	}

	mNextUnusedLink = &mBufferLink[0];
	mNextUsedLink = NULL;
}

template<class T>
PTyped<T>::~PTyped()
{
	for(unsigned int i = 0; i M AK_POOL_SIZE; i++) {
		delete mBufferLink[i].mData;
	}
}


template<class T>
T* PTyped<T>::GetNext()
{
	if(mNextUnusedLink == NULL) {
		return NULL;
	}

	PLinker* link = mNextUnusedLink;
	T* temp = (T*)link->mData;

	mNextUnusedLink = link->mNextLink;

	link->mNextLink = mNextUsedLink;
	mNextUsedLink = link;

	link->mData = NULL;
	return temp;
}

template<class T>
void PTyped<T>::Free(T* rPValue)
{
	if(mNextUsedLink == NULL) {
		delete rPValue;
		return;
	}

	PLinker* temp = mNextUsedLink->mNextLink;
	mNextUsedLink->mNextLink = mNextUnusedLink;
	mNextUnusedLink = mNextUsedLink;
	mNextUsedLink = temp;

	mNextUnusedLink->mData = rPValue;
}