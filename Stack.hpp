#ifndef STACK_H
#define STACK_H

// External
#include <stdlib.h>

// Internal
#include "Exception.hpp"

using namespace std;

template<typename T>

class Stack {
private:
	struct Node {
		Node() {
			prev = next = 0;
		}

		T val;
		Node *prev;
		Node *next;
	};


public:
	Stack(int aMaxSize = 2048) {
		maxSize = aMaxSize;
		size = 0;
		back = 0;
		front = 0;
	}

	~Stack() {
		while (back) {
			Node *prev = back->prev;
			delete back;

			back = prev;
		}
	}

	void push(T val) {
		if (++size >= maxSize) {
			throw StackOverflowException();
		}

		Node *node = new Node();
		node->val = val;
		node->prev = back;

		if (back) {
			back->next = node;
		}

		back = node;

		if (size == 1) {
			front = back;
		}
	}

	T pop() {
		if (--size < 0) {
			throw StackUnderflowException();
		}

		Node *node = back;
		back = back->prev;

		T ret = node->val;
		delete node;

		return ret;
	}

	T peek() {
		if (back != NULL) {
			return back->val;
		}

		throw StackUnderflowException("Naked stack-peek is illegal");
	}

	T peek(int index) {
		Node *node = front;

		for (int i=0; i<index && node; i++) {
			node = node->next;
		}

		return node->val;
	}

	int Size() {
		return size;
	}

private:
	int maxSize;
	int size;

	Node *front;
	Node *back;
};

#endif // STACK_H