#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Internal
#include "./Parser/Variable.h"
#include "./Compiler/Parser.h"

/***** Funbctions.h *****
* Contains the maping between opcode-functions and  libc/stdlibc++/custom functions.
* All Scrap-callable C-functions must have a Var* 
* return value, and zero or more Var* parameters.
* Example:
*		Var* f1();
*		Var* f2(Var*,Var*);
*****/

namespace AikiStd {
	void RegisterFunctions(Parser *aParser);

	int	GetParameterCount(uint aFuncID);

	Variable* CallStdFunc(uint ID, int aArgCount, Variable **aVariable);
	Variable* TestFunction(Variable *aVariable1, Variable *aVariable2);
	Variable* Print(Variable *aVariable);
};

#endif // FUNCTIONS_H