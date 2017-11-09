#ifndef STACK_H
#define STACK_H

// Internal
#include "Exception.h"

// External
#include <stdlib.h>


template<typename T> class Stack {
private:
	struct Node {
		Node() {
			prev = next = 0;
		}

		T val;
		Node *prev;
		Node *next;
	};

private:
	int mMaxSize;
	int mSize;
	int mLineNumber;

	MCString mFilename;
	MCString mName;

	//Function
	Node *mFront;
	Node *mBack;
	Function* mFunction; // Replaces node system with function class

public:
	Stack(int rMaxSize = 2048) : mMaxSize(rMaxSize), mSize(0), mBack(0),
		mFront(0), mLineNumber(0), mFunction(NULL)
	{

	}

	Stack() : mMaxSize(rMaxSize), mSize(0), mBack(0),
		mFront(0), mLineNumber(0), mFunction(NULL)
	{

	}

	~Stack() {
		while(mBack) {
			Node *prev = mBack->prev;
			delete mBack;

			mBack = prev;
		}
	}

	Function* GetFunction() const
	{
		return mFunction;
	}

	void Push(T rVal)
	{
		if(++mSize >= mMaxSize) {
			throw StackOverflowException();
		}

		Node *node = new Node();
		node->val = rVal;
		node->prev = mBack;

		if(mBack) {
			mBack->next = node;
		}

		mBack = node;

		if(mSize == 1) {
			mFront = mBack;
		}
	}

	T Pop()
	{
		if(--mSize < 0) {
			throw StackUnderflowException();
		}

		Node *node = mBack;
		mBack = mBack->prev;

		T ret = node->val;
		delete node;

		return ret;
	}

	T Peek()
	{
		if(mBack != NULL) {
			return mBack->val;
		}

		throw StackUnderflowException("Naked stack-peek is illegal");
	}

	T Peek(int rIndex) 
	{
		Node *node = mFront;

		for(int i = 0; i < rIndex && node; i++) {
			node = node->next;
		}

		return node->val;
	}

	int Size() {
		return mSize;
	}
};
#endif // STACK_H