#ifndef SCOPETYPE_H
#define SCOPETYPE_H

// Interal
#include "./Parser/Variable.h"
#include "./codes.h"
#include "./stack.h"

// External
#include <map>
#include <stdlib.h>
#include <string>

template<typename T>

class ScopeType {
	public:
		ScopeType() {
			varCounter = 1;
		}

		~ScopeType() {
			typename map<uint,T>::iterator it;

			for (it = vars.begin(); it != vars.end(); it++) {
				delete it->second;
			}

			while (joined.Size() > 0) {
				delete joined.Pop();
			}
		}

		uint GetItemID(T aValue) {
			for (int i=joined.Size()-1; i>=0; i--) {
				uint id = joined.Peek(i)->GetItemID(aValue);
				if (id != 0) {
					return id;
				}
			}

			typename map<uint,T>::iterator it;
			for (it = vars.begin(); it != vars.end(); it++) {
				if (*(it->second) == *aValue) {
					return it->first;
				}
			}

			return 0;
		}

		bool ItemExists(uint aID) {
			for (int i=joined.Size()-1; i>=0; i--) {
				if (joined.Peek(i)->ItemExists(aID)) {
					return true;
				}
			}

			if (vars.count(aID) != 0) {
				return true;
			}

			return false;
		}

		void AddItem(uint id, T t) {
			varCounter++;

			if (joined.Size()) {
				ScopeType<T> *nested = joined.Peek();
				nested->AddItem(id, t);
				return;
			}

			vars[id] = t;
		}

		uint GetVarCounter() {
			return varCounter;
		}

		T GetVar(uint aID) {
			for (int i=joined.Size()-1; i>=0; i--) {
				T t = joined.Peek(i)->GetVar(aID);
				if (t) {
					return t;
				}
			}

			if (vars.count(aID) == 0) {
				return NULL;
			}

			return vars[aID];
		}


		int NestCount() {
			return joined.Size();
		}

		void PushNestedScope() {
			ScopeType<T> *nested = new ScopeType<T>();
			joined.Push(nested);
		}

		void PopNestedScope() {
			delete joined.Pop();
		}

	protected:
		map<uint,T> vars;
		Stack<ScopeType<T>*> joined;
		uint varCounter;
};
#endif

/* ScopeType 
* Default scope, used at runtime. 
* ID's are mapped directly to the variable objects
*/
typedef ScopeType<Variable*> Scope;

/*	CompileScope
* Scope used for scope checking during compilation
* IDs are mapped to variable names rather than actual variables
*/
typedef ScopeType<string*> CompileScope;