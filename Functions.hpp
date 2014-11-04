#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Internal
#include "./Parser/Variable.hpp"
#include "./Compiler/Parser.hpp"

/***** Funbctions.hpp *****
* Contains the maping between opcode-functions and  libc/stdlibc++/custom functions.
* All Scrap-callable C-functions must have a Var* 
* return value, and zero or more Var* parameters.
* Example:
*		Var* f1();
*		Var* f2(Var*,Var*);
*****/

namespace AikiStd {
	void registerFunctions(Parser *aParser);

	int	getParameterCount(uint aFuncID);

	Variable* callStdFunc(uint ID, int aArgCount, Variable **aVariable);
	Variable* testFunction(Variable *aVariable1, Variable *aVariable2);
	Variable* print(Variable *aVariable);
};

#endif // FUNCTIONS_H