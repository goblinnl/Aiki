#ifndef MIXER_C_STRING_H
#define MIXER_C_STRING_H

// Internal
#include "MCArray.h"

class MCString {

public:
	char* mBuffer;
	static char* mEmpty;

	MCString();
	MCString(double rNumber);
	MCString(char rVal);
	MCString(const char* rVal);
	MCString(MCString* rVal);
	MCString(const MCString &rCopy);
	MCString(MCString&& rMove);
	~MCString();

	char* GetEmpty();
	void SetEmpty(char* const &rEmpty);

	MCString Trim();
	MCString TrimLeft();
	MCString TrimRight();
	MCString Trim(char rChar);
	MCString TrimLeft(char rChar);
	MCString TrimRight(char rChar);
	MCString Replace(const MCString &rNeedle, const MCString &rReplace);
	MCString Reverse();
	MCString ToLower();
	MCString ToUpper();
	MCString Substring(int rStart);
	MCString Substring(int rStart, int rLengt);


	static MCString Format(const char* rFormat, ...);

	void SetFormat(const char* rFormat, ...);
	void AppendFormat(const char* rFormat, ...);
	void CopyToBuffer(const char* rBuffer);
	void AppendToBuffer(const char* rBuffer);
	void AppendToBuffer(const char* rBuffer, int rCounter);
	void AppendToBuffer(char rBuffer);
	void Split(const MCString &rNeedle, MCArray<MCString> &rResult);

	int Count(const MCString &rNeedle);
	int Length();
	int IndexOf(char rChar);
	int IndexOf(char rChar, int rPos);
	int LastIndexOf(char rChar);
	int LastIndexOf(char rChar, int rPos);
	int IndexOf(MCString rTxt, int rPos);
	int LastIndexOf(MCString rTxt, int rPos);

	bool Contains(const MCString &rPoint);
	bool StartsWith(const MCString &rPoint);
	bool EndsWith(const MCString &rPoint);
	bool StartsWith(const char rPoint);
	bool EndsWith(const char rPoint);

	char* StringReverse(char* rString);
	char* StringToLower(char *rString);
	char* StringToUpper(char *rString);

	operator const char*();
	operator const char*() const;

	MCString& operator=(char* rChar);
	MCString& operator=(const char* rChar);
	MCString& operator=(const MCString &rStr);


	MCString& operator+=(const char* rChar);
	MCString& operator+=(const char rChar);

	bool operator==(const char* rChar);
	bool operator!=(const char* rChar);

	char& operator[](int rIndex);
};

MCString operator+(const MCString &rLHS, const char* rRHS);
MCString operator+(const MCString &rLHS, const char rRHS);

#endif // MIXER_C_STRING_H