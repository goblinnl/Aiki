#ifndef MIXER_C_SSTREAM
#define MIXER_C_SSTREAM

// Internal
#include "MCommon.h"
#include "MCString.h"

enum ENewLineMode {
	_CRLF,
	_LF,
	_CR
};

class MCStream {
public:
	ENewLineMode mNewLineMode;

public:
	MCStream();
	~MCStream();

	virtual unsigned long Lenght() = 0;
	virtual unsigned long Location() = 0;
	virtual void Seek(unsigned long rPos, int rLoc) = 0;
	virtual void Close();

	virtual void* Read(unsigned long rLenght) = 0;
	MCString ReadLine();
	void* ReadToEnd();
	int ReadInt();
	long ReadLong();
	float ReadFloat();
	double ReadDouble();
	MCString ReadString();

	virtual void Write(const void *rPoint, unsigned long rLenght) = 0;
	void Writeline(const MCString &rStr);
	void WriteInt(const int &rIndex);
	void WriteLong(const long &rIndex);
	void WriteFloat(const float &rIndex);
	void WriteDouble(const double &rIndex);
	void WriteString(const MCString &rStr);
	void WriteStream(MCStream &rStream);


	MCStream& operator >>(int &rInt);
	MCStream& operator >>(long &rLong);
	MCStream& operator >>(float &rFloat);
	MCStream& operator >>(double &rDouble);
	MCStream& operator >>(MCString &rStr);

	MCStream& operator <<(int rInt);
	MCStream& operator <<(long rLong);
	MCStream& operator <<(float rFloat);
	MCStream& operator <<(double rDouble);
	MCStream& operator <<(MCString rStr);
	MCStream& operator <<(MCStream &rStream);
};

#endif // MIXER_C_SSTREAM