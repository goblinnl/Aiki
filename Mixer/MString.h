#ifndef MIXER_STRING_H
#define MIXER_STRING_H

// Internal
#include "MArray.h"

class MString {

public:
	char* mBuffer;
	char* mEmpty;

	MString();
	MString(double rNumber);
	MString(char rVal);
	MString(const char* rVal);
	MString(MString* rVal);
	MString(const MString &rCopy);
	MString(MString&& rMove);
	~MString();

	char* GetEmpty();
	void SetEmpty(char* const &rEmpty);	
	
	MString Trim();
	MString TrimLeft();
	MString TrimRight();
	MString Trim(char rChar);
	MString TrimLeft(char rChar);
	MString TrimRight(char rChar);
	MString Replace(const MString &rNeedle, const MString &rReplace);
	MString Reverse();
	MString ToLower();
	MString ToUpper();
	MString Substring(int rStart);
	MString Substring(int rStart, int rLengt);
	

	static MString Format(const char* rFormat, ...);

	void SetFormat(const char* rFormat, ...);
	void AppendFormat(const char* rFormat, ...);
	void CopyToBuffer(const char* rBuffer);
	void AppendToBuffer(const char* rBuffer);
	void AppendToBuffer(const char* rBuffer, int rCounter);
	void AppendToBuffer(char rBuffer);
	void Split(const MString &rNeedle, MArray<MString> &rResult);

	int Count(const MString &rNeedle);
	int Size();	
	int IndexOf(char rChar);
	int IndexOf(char rChar, int rPos);
	int LastIndexOf(char rChar);
	int LastIndexOf(char rChar, int rPos);
	int IndexOf(MString rTxt, int rPos);
	int LastIndexOf(MString rTxt, int rPos);

	bool Contains(const MString &rPoint);
	bool StartsWith(const MString &rPoint);
	bool EndsWith(const MString &rPoint);
	bool StartsWith(const char rPoint);
	bool EndsWith(const char rPoint);

	char* StringReverse(char* rString);
	char* StringToLower(char *rString);
	char* StringToUpper(char *rString);

	operator const char*();
	operator const char*() const;

	MString& operator=(char* rChar);
	MString& operator=(const char* rChar);
	MString& operator=(const MString &rStr);


	MString& operator+=(const char* rChar);
	MString& operator+=(const char rChar);

	bool operator==(const char* rChar);
	bool operator!=(const char* rChar);

	char& operator[](int rIndex);
};

MString operator+(const MString &rLHS, const char* rRHS);
MString operator+(const MString &rLHS, const char rRHS);

#endif // MIXER_STRING_H