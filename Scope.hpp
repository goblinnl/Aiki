#ifndef SCOPETYPE_H
#define SCOPETYPE_H

// Interal
#include "./Parser/Variable.hpp"
#include "./codes.hpp"
#include "./stack.hpp"

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
				delete joined.pop();
			}
		}

		uint getItemID(T aValue) {
			for (int i=joined.Size()-1; i>=0; i--) {
				uint id = joined.peek(i)->getItemID(aValue);
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

		bool itemExists(uint aID) {
			for (int i=joined.Size()-1; i>=0; i--) {
				if (joined.peek(i)->itemExists(aID)) {
					return true;
				}
			}

			if (vars.count(aID) != 0) {
				return true;
			}

			return false;
		}

		void addItem(uint id, T t) {
			varCounter++;

			if (joined.Size()) {
				ScopeType<T> *nested = joined.peek();
				nested->addItem(id, t);
				return;
			}

			vars[id] = t;
		}

		uint getVarCounter() {
			return varCounter;
		}

		T getVar(uint aID) {
			for (int i=joined.Size()-1; i>=0; i--) {
				T t = joined.peek(i)->getVar(aID);
				if (t) {
					return t;
				}
			}

			if (vars.count(aID) == 0) {
				return NULL;
			}

			return vars[aID];
		}


		int nestCount() {
			return joined.Size();
		}

		void pushNestedScope() {
			ScopeType<T> *nested = new ScopeType<T>();
			joined.push(nested);
		}

		void popNestedScope() {
			delete joined.pop();
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
* Scope used for scope checking during compiliation
* IDs are mapped to variable names rather than actual variables
*/
typedef ScopeType<string*> CompileScope;