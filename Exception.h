#ifndef EXCEPTION_H
#define EXCEPTION_H

/***** Exception Definitions *****
* Exceptions are created with macros to prevent clutter.
*****/
// Internal
#include "Mixer/MCommon.h"
#include "Mixer/MCString.h"


// External
#include <exception>
#include <stdexcept>
#include <string>

//using namespace std;


#define EXCEPTION_BEGIN(_NAME, _TXTDEFAULT)		\
	class _NAME : public std::runtime_error {	\
	public: 									\
	_NAME(MCString txt = _TXTDEFAULT)			\
			:std::runtime_error(txt){}			\
	const char *what() {						\
		MCString str = #_NAME;					\
		str += ": ";							\
		str += runtime_error::what();			\
		return str;								\
		}

#define EXCEPTION_END	\
		};

EXCEPTION_BEGIN(InternalErrorException, "Internal error")
EXCEPTION_END

EXCEPTION_BEGIN(VarAlreadyDefinedException, "Variable is already defined")
EXCEPTION_END

EXCEPTION_BEGIN(FuncNotDefinedException, "Function is not defined")
EXCEPTION_END

EXCEPTION_BEGIN(FuncAlreadyDefinedException, "Function is already defined!")
EXCEPTION_END

EXCEPTION_BEGIN(InvalidOpException, "Invalid operation")
EXCEPTION_END

EXCEPTION_BEGIN(VarNotDefinedException, "Variable not defined")
EXCEPTION_END

EXCEPTION_BEGIN(InvalidTokenException, "Invalid token")
EXCEPTION_END

EXCEPTION_BEGIN(SyntaxErrorException, "Syntax error")
EXCEPTION_END

EXCEPTION_BEGIN(NotImplementedException, "Feature not implemented")
EXCEPTION_END

EXCEPTION_BEGIN(StackOverflowException, "Stack overflow")
EXCEPTION_END

EXCEPTION_BEGIN(StackUnderflowException, "Stack underflow")
EXCEPTION_END

EXCEPTION_BEGIN(InvalidArgumentException, "Invalid argument passed to function")
EXCEPTION_END

EXCEPTION_BEGIN(FileNotFoundException, "File does not exist")
EXCEPTION_END

EXCEPTION_BEGIN(NullPointerException, "Null-pointer exception")
EXCEPTION_END

#endif // EXCEPTION