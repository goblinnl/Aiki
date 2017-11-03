#ifndef MIXER_C_COMMON_H
#define MIXER_C_COMMON_H

// Internal
#include "MCMath.h"

// External
#include <cstdio>
#include <cmath>
#include <cassert>
#include <cstdarg>
#include <cstdlib>

// Define Boolean Tables
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL 
#define NULL 0
#endif

#ifndef ASSERT
#define ASSERT(rExpr) assert(rExpr)
#endif


// Safe snprintf
#if defined(_MSC_VER) && _MSC_VER < 1900

#define snprintf c99_snprintf
#define vsnprintf c99_vsnprintf

inline int c99_vsnprintf(char *rOutBuf, size_t rSize, const char *rFormat, va_list rAp)
{
	int count = -1;

	if(rSize != 0)
		count = _vsnprintf_s(rOutBuf, rSize, _TRUNCATE, rFormat, rAp);
	if(count == -1)
		count = _vscprintf(rFormat, rAp);

	return count;
}

inline int c99_snprintf(char *rOutBuf, size_t rSize, const char *rFormat, ...)
{
	int count;
	va_list ap;

	va_start(ap, rFormat);
	count = c99_vsnprintf(rOutBuf, rSize, rFormat, ap);
	va_end(ap);

	return count;
}
#endif

#endif // MIXER_C_COMMON_H