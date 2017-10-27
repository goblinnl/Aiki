#include "Functions.h"
#include "./Compiler/Function.h"
#include <stdarg.h>

namespace AikiStd {
	struct StdFunc {
		int params;
		void *func;
	};

	map<uint,StdFunc> stdFuncMap;
	
	void registerFunction(Parser *parser, void *func, string name, int paramCount) {
		FunctionSignature funcSign(name, paramCount);

		uint id = parser->registerStdFunction(funcSign);

		StdFunc f;
		f.params = paramCount;
		f.func = func;
		stdFuncMap[id] = f;
	}

	void registerFunctions(Parser *parser) {
		registerFunction(parser, (void*)&print,	"print",	1);
	}
	
	Variable* scrapStdCall(void *func, int pCount, Variable **params) {
		Variable *retVal = NULL;

		for (int i = pCount-1; i >= 0; i--) {
			void *paramPtr = params[i];
			
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
			call	func
			mov		retVal, eax
		}
		#elif defined(__GNUC__) 
		asm volatile("call *0\n" :: "r"(func) );
		#endif

		for (int i=0; i<pCount; i++) {
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

	int getParameterCount(uint funcId) {
		return stdFuncMap[funcId].params;
	}

	Variable* callStdFunc(uint id, int argCount, Variable **params) {
		StdFunc f = stdFuncMap[id];
		return scrapStdCall(f.func, f.params, params);
	}
	
	Variable* print(Variable *var) {
		printf("%i\n", var->getInteger());
		return NULL;
	}
}