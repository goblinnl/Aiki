#ifndef SCOPETYPE_H
#define SCOPETYPE_H

// Internal
#include "./Parser/Variable.h"
#include "./codes.h"
#include "./stack.h"

// External
#include <map>
#include <stdlib.h>
#include <string>

template<typename T>

class ScopeType {
protected:
	std::map<uint, T> mVars;
	Stack<ScopeType<T>*> mJoined;
	uint mVarCounter;

public:
	ScopeType() {
		mVarCounter = 1;
	}

	~ScopeType() {
		typename std::map<uint, T>::iterator it;

		for(it = mVars.begin(); it != mVars.end(); it++) {
			delete it->second;
		}

		while(mJoined.Size() > 0) {
			delete mJoined.Pop();
		}
	}

	uint GetItemID(T rValue) {
		for(int i = mJoined.Size() - 1; i >= 0; i--) {
			uint id = mJoined.Peek(i)->GetItemID(rValue);
			if(id != 0) {
				return id;
			}
		}

		typename std::map<uint, T>::iterator it;
		for(it = mVars.begin(); it != mVars.end(); it++) {
			if(*(it->second) == *rValue) {
				return it->first;
			}
		}

		return 0;
	}

	bool ItemExists(uint rID) {
		for(int i = mJoined.Size() - 1; i >= 0; i--) {
			if(mJoined.Peek(i)->ItemExists(rID)) {
				return true;
			}
		}

		if(mVars.count(rID) != 0) {
			return true;
		}

		return false;
	}

	void AddItem(uint rID, T rTemp) {
		mVarCounter++;

		if(mJoined.Size()) {
			ScopeType<T> *nested = mJoined.Peek();
			nested->AddItem(rID, rTemp);
			return;
		}

		mVars[rID] = rTemp;
	}

	uint GetVarCounter() {
		return mVarCounter;
	}

	T GetVar(uint rID) {
		for(int i = mJoined.Size() - 1; i >= 0; i--) {
			T temp = mJoined.Peek(i)->GetVar(rID);
			if(temp) {
				return temp;
			}
		}

		if(mVars.count(rID) == 0) {
			return NULL;
		}

		return mVars[rID];
	}

	int NestCount() {
		return mJoined.Size();
	}

	void PushNestedScope() {
		ScopeType<T> *nested = new ScopeType<T>();
		mJoined.Push(nested);
	}

	void PopNestedScope() {
		delete mJoined.Pop();
	}
};
#endif // SCOPETYPE_H

/* ScopeType
* Default scope, used at runtime.
* ID's are mapped directly to the variable objects
*/
typedef ScopeType<Variable*> Scope;

/*	CompileScope
* Scope used for scope checking during compilation
* IDs are mapped to variable names rather than actual variables
*/
typedef ScopeType<MString*> CompileScope;