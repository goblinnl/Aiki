#include "MCStream.h"

MCStream::MCStream() : mNewLineMode(_LF)
{

}

MCStream::~MCStream()
{
	Close();
}

MCString MCStream::ReadLine()
{
	MCString str;
	char c;
	do {
		c = *(char*)Read(sizeof(char));

		if(c == '\r') {
			continue;
		}

		if(c != '\n') {
			str += c;
		}

	} while(c != '\n');

	return str;
}

void* MCStream::ReadToEnd()
{
	return Read(Lenght() - Location());
}

int MCStream::ReadInt()
{
	return *(int*)Read(sizeof(int));
}

long MCStream::ReadLong()
{
	return *(long *)Read(sizeof(long));
}

float MCStream::ReadFloat()
{
	return *(float*)Read(sizeof(float));
}

double MCStream::ReadDouble()
{
	return *(double*)Read(sizeof(double));
}

MCString MCStream::ReadString()
{
	MCString temp;
	char c; {
		c = *(char*)Read(sizeof(char));
		if(c != '\0') {
			temp += c;
		}
	}while(c != '\0');

	return temp;
}

void MCStream::Writeline(const MCString &rStr)
{
	// Write
	Write((const char*)rStr, strlen(rStr));

	// Write New Line
	switch(mNewLineMode) {
	case _CRLF: Write("\r\n", 2);
		break;
	case _LF:
		Write("\n", 1);
		break;
	case _CR:
		Write("\r", 1);
		break;
	}
}

void MCStream::WriteInt(const int &rIndex)
{
	return Write(&rIndex, sizeof(int));
}

void MCStream::WriteLong(const long &rIndex)
{
	return Write(&rIndex, sizeof(long));
}

void MCStream::WriteFloat(const float &rIndex)
{
	return Write(&rIndex, sizeof(float));
}

void MCStream::WriteDouble(const double &rIndex)
{
	return Write(&rIndex, sizeof(double));
}

void MCStream::WriteString(const MCString &rStr)
{
	Write((const char*)rStr, strlen(rStr) + 1);
}

void MCStream::WriteStream(MCStream &rStream)
{
	unsigned long pos = rStream.Location();
	const void* buffer = rStream.ReadToEnd();
	Write(buffer, rStream.Lenght() - pos);
}

MCStream& MCStream::operator>>(int &rInt)
{
	rInt = ReadInt();
	return *this;
}

MCStream& MCStream::operator>>(long &rLong)
{
	rLong = ReadLong();
	return *this;
}

MCStream& MCStream::operator>>(float &rFloat)
{
	rFloat = ReadFloat();
	return *this;
}

MCStream& MCStream::operator>>(double &rDouble)
{
	rDouble = ReadDouble();
	return *this;
}

MCStream& MCStream::operator>>(MCString &rStr)
{
	rStr = ReadString();
	return *this;
}

MCStream& MCStream::operator<<(int rInt)
{
	WriteInt(rInt);
	return *this;
}

MCStream& MCStream::operator<<(long rLong)
{
	WriteLong(rLong);
	return *this;
}

MCStream& MCStream::operator<<(float rFloat)
{
	WriteFloat(rFloat);
	return *this;
}

MCStream& MCStream::operator<<(double rDouble)
{
	WriteDouble(rDouble);
	return *this;
}

MCStream& MCStream::operator<<(MCString rStr)
{
	WriteString(rStr);
	return *this;
}

MCStream& MCStream::operator<<(MCStream &rStream)
{
	WriteStream(rStream);
	return *this;
}