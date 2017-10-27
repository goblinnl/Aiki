#ifndef MARRAY_H
#define MARRAY_H

// Internal
#include "MCommon.h"

// External
#include <cstdlib>
#include <cstring>

// Number of slots in the stack array
#define SLOTS = 512

template <class T> class MArray 
{

public:
	T** mItems;
	int mSlots;
	int mUsed;

public:
	MArray();
	MArray(const MArray& rCopy);
	~MArray();

	T& Push(); // Push on the stack
	T* Pop(); // Pop From the stack
	T* PopAt(int rIndex); // Pop At index
	 
	void PopAll(); // Pop all objects from the stack
	void ClearAll(); // Clear all object from the stack
	int Count(); // Count all objects in the stack

	T& operator[](int rIndex); // Get stack at index
	T& Get(int rIndex); // Get Stack at index

private:
	void AllocSlots(int rSlots); // Allocate slots for stack

};
#endif