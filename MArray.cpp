#include "MArray.h"

template<class T>
MArray<T>::MArray() : mItems(NULL), mSlots(0), mUsed(0)
{
	AllocSlots(SLOTS);
}

template <class T>
MArray<T>::~MArray()
{
	ClearAll();

	if(mItems != NULL)
		free(mItems);
}

template <class T>
MArray<T>::MArray(const MArray& rCopy)
{
	mItems = NULL;
	mSlots = 0;
	mUsed = 0;
	
	AllocSlots(rCopy.mSlots);

	mUsed = rCopy.mUsed;

	for(unsigned int i = 0; i < mUsed; i++) {
		mItems[i] = new T;
		mItems[i] = *rCopy.mItems[i];
	}
}


template <class T>
void MArray<T>::AllocSlots(int rSlots)
{
	mSlots += rSlots;
	T** pMemory = (T**)malloc(sizeof(T*) * mSlots);

	if(mItems != NULL) {
		memcpy(pMemory, mItems, sizeof(T*) * (mSlots - rSlots));
		free(mItems);
	}

	// Update the pointer
	mItems = pMemory;
}


template <class T>
T& MArray<T>::Get(int rIndex)
{
	ASSERT(rIndex >= 0 && rIndex < mUsed);
	return *mItems[rIndex];
}

template <class T>
T& MArray<T>::operator[](int rIndex)
{
	ASSERT(rIndex >= && rIndex < mUsed);
	return *mItems[rIndex];
}

template <class T>
int MArray<T>::Count()
{
	return mUsed;
}

template <class T>
void MArray<T>::ClearAll()
{
	for(unsigned int i = 0; i < mUsed; i++) {
		delete mItems[i];
		mItems[i] = NULL;
	}

	mUsed = 0; // Reset Counter
}

template <class T>
void MArray<T>::PopAll()
{
	for(unsigned int i = 0; i < mUsed; i++) {
		mItems[i] = NULL;
	}
	mUsed = 0;
}

template <class T>
T* MArray<T>::PopAt(int rIndex)
{
	mUsed--;

	T* object = mItems[rIndex];
	
	for(unsigned int i = rIndex; i < mUsed; i++) {
		mItems[i] = mItems[i + 1];
	}
	mItems[mUsed] = NULL;

	return object;
}

template <class T>
T* MArray<T>::Pop()
{
	mUsed--;

	T* object = mItems[mUsed];
	return object;
}

template <class T>
T& MArray<T>::Push()
{
	if(mUsed >= mSlots) {
		AllocSlots(SLOTS);
	}
}



