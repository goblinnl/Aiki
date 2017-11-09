#include "Functions.h"
#include "./Compiler/Function.h"
#include <stdarg.h>

namespace AikiStd {
	struct StdFunc {
		int params;
		void *func;
	};

	std::map<uint, StdFunc> stdFuncMap;

	void registerFunction(ArgParser *rParser, void *rFunc, MCString rName, int rParamCount) {
		FunctionSignature funcSign(rName, rParamCount);

		uint id = rParser->RegisterStdFunction(funcSign);

		StdFunc f;
		f.params = rParamCount;
		f.func = rFunc;
		stdFuncMap[id] = f;
	}

	void RegisterFunctions(ArgParser *rParser) {
		registerFunction(rParser, (void*)&Print, "print", 1);
	}

	Variable* scrapStdCall(void *rFunc, int rParamCount, Variable **rParams) {
		Variable *retVal = NULL;

		for(int i = rParamCount - 1; i >= 0; i--) {
			void *paramPtr = rParams[i];

#ifdef _MSC_VER
			__asm
			{
				mov		eax, paramPtr
					push	eax
			}
#elif defined(__GNUC__)
			asm("push $0\n" : "=r" (paramPtr));
#endif
		}

#ifdef _MSC_VER
		__asm
		{
			call	rFunc
				mov		retVal, eax
		}
#elif defined(__GNUC__) 
		asm volatile("call *0\n" :: "r"(func) );
#endif

		for(int i = 0; i < rParamCount; i++) {
#ifdef _MSC_VER
			__asm
			{
				pop		ebx
			}
#elif defined(__GNUC__)
			__asm("pop %bx");
#endif
		}

		return retVal;
	}

	int GetParameterCount(uint rFuncID) {
		return stdFuncMap[rFuncID].params;
	}

	Variable* CallStdFunc(uint rID, int rArgCount, Variable **rParams) {
		StdFunc f = stdFuncMap[rID];
		return scrapStdCall(f.func, f.params, rParams);
	}

	Variable* Print(Variable *rVar) {
		printf("%i\n", rVar->GetInteger());
		return NULL;
	}
}