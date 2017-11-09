#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Internal
#include "Parser/Variable.h"
#include "Compiler/ArgParser.h"

/***** Funbctions.h *****
* Contains the mapping between opcode-functions and  libc/stdlibc++/custom functions.
* All Scrap-callable C-functions must have a Var*
* return value, and zero or more Var* parameters.
* Example:
*		Var* f1();
*		Var* f2(Var*,Var*);
*****/

namespace AikiStd {
	void RegisterFunctions(ArgParser *rParser);

	int	GetParameterCount(uint rFuncID);

	Variable* CallStdFunc(uint rID, int rArgCount, Variable **rVar);
	Variable* TestFunction(Variable *rVar1, Variable *rVar2);
	Variable* Print(Variable *rVar);
};
#endif // FUNCTIONS_H