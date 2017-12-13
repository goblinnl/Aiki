#ifndef BYTE_PARSER_H
#define BYTE_PARSER_H

#include "../Global.h"
#include "../Codes.h"
#include "Function.h"


class ByteParser {
private:
	int mPosition;
	Function* mFunction;
	MCString* mStringTable;
	unsigned char* mCode;
	int mCount;
	int mBufferSize;
	byte* mBuffer;
	MCList<MCString*>* mStringsTable;

public:
	ByteParser();
	~ByteParser();

	MCString& ReadString();
	MCList<MCString>* ReadStrings();

	unsigned char ReadByte();
	unsigned char* ReadBytes(int rSize);

	int ReadInt();
	short ReadShort();
	double ReadDouble();
	bool ReadBool();

	int GetPosition();
	Function* GetFunction() const;
	MCString* GetStringTable() const;
	unsigned char* GetCode() const;

	void SetCode(unsigned char* rCode);
	void SetStringTable(MCString* rStringTable);
	void SetFunction(Function* rFunction);
	void SetPosition(int rPosition);



	// New Part Temp Set
	void WriteOperator(const byte& rOperator);
	void WriteBool(int rValue);
	void WriteByte(byte rValue);
	void WriteBytes(byte* rValue, int rSize, bool rFull);
	void WriteShort(short rValue);
	void WriteInt(int rValue);
	void WriteDouble(double rValue);
	void WriteString(const MCString& rValue);
	void WriteStrings(MCList<MCString> rValue, bool rFull);
	void WriteStrings(MCArray<MCString>& rValue, bool rFull);
	void DeleteAll();

private:
	void GetSize(int rAlloc);
	void AllocMoreMemory(int rSize);
};
#endif // BYTE_PARSER_H