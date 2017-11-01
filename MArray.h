#ifndef MIXER_ARRAY_H
#define MIXER_ARRAY_H

// Internal
#include "MCommon.h"

// External
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdarg>

template<class T> class MArray
{
public:
	T** mItems;
	int mSlots;
	int mUsed;

public:
	
	MArray() : mItems(NULL), mSlots(0), mUsed(0)
	{
		AllocSlots(256);
	}

	MArray(const MArray& rCopy)
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

	~MArray()
	{
		ClearAll();

		if(mItems != NULL) {
			free(mItems);
		}
	}

	// Push on the stack
	T& Push()
	{
		if(mUsed >= mSlots) {
			AllocSlots(256);
		}

		T* object = new T;
		mItems[mUsed] = object;

		mUsed++;

		return *object;
	}

	// Pop From the stack
	T* Pop()
	{
		mUsed--;

		T* object = mItems[mUsed];
		mItems[mUsed] = NULL;

		return object;
	}

	// Pop At index
	T* PopAt(int rIndex)
	{
		mUsed--;

		T* object = mItems[rIndex];

		for(unsigned int i = rIndex; i < mUsed; i++) {
			mItems[i] = mItems[i + 1];
		}
		mItems[mUsed] = NULL;

		return object;
	}

	// Pop all objects from the stack
	void PopAll()
	{
		for(unsigned int i = 0; i < mUsed; i++) {
			mItems[i] = NULL;
		}
		mUsed = 0;
	}

	// Clear all object from the stack
	void ClearAll()
	{
		for(unsigned int i = 0; i < mUsed; i++) {
			delete mItems[i];
			mItems[i] = NULL;
		}

		mUsed = 0; // Reset Counter
	}

	// Count all objects in the stack
	int Count()
	{
		return mUsed;
	}

	// Get stack at index
	T& operator[](int rIndex)
	{
		ASSERT(rIndex >= && rIndex < mUsed);
		return *mItems[rIndex];
	}

	// Get Stack at index
	T& Get(int rIndex)
	{
		ASSERT(rIndex >= 0 && rIndex < mUsed);
		return *mItems[rIndex];
	}

private:
	void AllocSlots(int rSlots)
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
};

#endif