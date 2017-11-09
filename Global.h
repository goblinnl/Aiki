#ifndef GLOBAL_H
#define GLOBAL_H


// Config
#define AIKI_VERSION 1 // Version of scripting language
#define AK_POOL_SIZE 4098 // Mal pool size of variables 
#define AK_CALLSTACK_SIZE 1024 // Max Call stack size
#define AK_ARGUMENT_SIZE 64 // Max number of arguments
#define AK_PI 3.14159265358979323846 // Define PI

#define AK_FUNCTION(rFunction) Variable* rFunction(ArgParser* rParser, ArgumentData* rArgs)
#define AK_FUNCTION_CONSTRUCTOR(rFunction) void* rFunction(ArgParser* rParser, ArgumentData* rArgs, Variable* rVar)
#define AK_FUNCTION_DESTRUCTOR(rFunction) void rFunction(ArgParser* rParser, void* rData)
#define AK_FUNCTION_TO_INDEX(rOperator) (int)rOperator - (int)AO_START - 1

// Reference Counter (Debugging)
#define AK_REFERENCE_INCREASE(rVar) (rVar)->IncreaseReferenceCounter()
#define AK_REFERENCE_DECREASE(rVar) (rVar)->DecreaseReferenceCounter()

// Easy access to functions
typedef Variable* (*AKFunction)(ArgParser* rParser, ArgumentData* rArgs);
typedef void* (*AKFunctionConstructor)(ArgParser* rParser, ArgumentData* rArgs, Variable* rVar);
typedef void(*AKFunctionDeconstructor)(ArgParser* rParser, void* mData);
typedef Variable* (*AKGetFunction)(ArgParser* rParser, void* rPointer);
typedef void(*AKSetFunction)(ArgParser* rParser, void* rPointer, Variable* rVar);

#endif // GLOBAL_H