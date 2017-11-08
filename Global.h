#ifndef GLOBAL_H
#define GLOBAL_H


// Config
#define AIKI_VERSION 1 // Version of scripting language
#define AK_POOL_SIZE 4098 // Mal pool size of variables 
#define AK_CALLSTACK_SIZE 1024 // Max Call stack size
#define AK_ARGUMENT_SIZE 64 // Max number of arguments
#define AK_PI 3.14159265358979323846 // Define PI

#define AK_FUNCTION(rFunction) Variable* rFunction(Instance)

#endif // GLOBAL_H