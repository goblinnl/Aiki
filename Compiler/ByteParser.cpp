#include "ByteParser.h"

ByteParser::ByteParser() : mPosition(0), mFunction(nullptr), mStringTable(nullptr), 
mCode(nullptr), mCount(0), mBufferSize(0), mBuffer(NULL), mStringsTable(NULL)
{

}

ByteParser::~ByteParser()
{
	DeleteAll();
}

MCList<MCString>* ByteParser::ReadStrings()
{
	MCList<MCString>* list = new MCList<MCString>();

	int size = ReadInt();
	for(int i = 0; i < size; i++) {
		list->Add(ReadString());
	}

	return list;
}

unsigned char ByteParser::ReadByte()
{
	return mCode[mPosition++];
}

unsigned char* ByteParser::ReadBytes(int rSize)
{
	unsigned char* temp = new unsigned char[rSize];
	memcpy(temp, mCode + mPosition, rSize);
	mPosition += rSize;

	return temp;
}

int ByteParser::ReadInt()
{
	int temp = 0;

	memcpy(&temp, mCode + mPosition, 4);
	mPosition += 4;

	return temp;
}

short ByteParser::ReadShort()
{
	short temp = 0;
	
	memcpy(&temp, mCode + mPosition, 2);
	mPosition += 2;

	return temp;
}

double ByteParser::ReadDouble()
{
	double temp = 0;
	memcpy(&temp, mCode + mPosition, 8);
	mPosition += 8;

	return temp;
}

bool ByteParser::ReadBool()
{
	return mCode[mPosition++] == 1;
}

Function* ByteParser::GetFunction() const
{
	return mFunction;
}

MCString* ByteParser::GetStringTable() const
{
	return mStringTable;
}

unsigned char* ByteParser::GetCode() const
{
	return mCode;
}

void ByteParser::SetCode(unsigned char* rCode)
{
	mCode = rCode;
}

void ByteParser::SetStringTable(MCString* rStringTable)
{
	mStringTable = rStringTable;
}

void ByteParser::SetFunction(Function* rFunction)
{
	mFunction = rFunction;
}

void ByteParser::SetPosition(int rPosition)
{
	mPosition = rPosition;
}

int ByteParser::GetPosition()
{
	return mPosition;
}

MCString& ByteParser::ReadString()
{
	return mStringTable[ReadShort()];
}

void ByteParser::WriteOperator(const byte& rOperator)
{
	GetSize(1);
	mBuffer[mCount++] = (byte)rOperator;
}

void ByteParser::WriteBool(int rValue)
{
	GetSize(1);
	mBuffer[mCount++] = rValue ? 1 : 0;
}

void ByteParser::WriteByte(byte rValue)
{
	GetSize(1);
	mBuffer[mCount++] = rValue;
}

void ByteParser::WriteBytes(byte* rValue, int rSize, bool rFull)
{
	GetSize(rSize + (rFull ? 4 : 0));
	if(rFull) {
		WriteInt(rSize);
	}

	memcpy(mBuffer + mCount, rValue, rSize);
	mCount += rSize;
}

void ByteParser::WriteShort(short rValue)
{
	GetSize(2);
	memcpy(mBuffer + mCount, &rValue, 2);
	mCount += 2;
}

void ByteParser::WriteInt(int rValue)
{
	GetSize(2);
	memcpy(mBuffer + mCount, &rValue, 4);
	mCount += 4;
}

void ByteParser::WriteDouble(double rValue)
{
	GetSize(8);
	memcpy(mBuffer + mCount, &rValue, 8);
	mCount += 8;
}

void ByteParser::WriteString(const MCString& rValue)
{
	for(int i = 0; i < mStringsTable->GetCount(); i++) {
		if(*mStringsTable->Get(i) == rValue) {
			WriteShort(i);
			return;
		}
	}

	mStringsTable->Add(new MCString(rValue));
	WriteShort(mStringsTable->GetCount() - 1);
}

void ByteParser::WriteStrings(MCList<MCString> rValue, bool rFull)
{
	if(rFull) {
		WriteInt(rValue.GetCount());
	}

	for(int i = 0; i < rValue.GetCount(); i++) {
		WriteString(rValue[i]);
	}

}

void ByteParser::WriteStrings(MCArray<MCString>& rValue, bool rFull)
{
	if(rFull) {
		WriteInt(rValue.Count());
	}

	for(int i = 0; i < rValue.Count(); i++) {
		WriteString(rValue[i]);
	}

}

void ByteParser::DeleteAll()
{
	mCount = 0;
	mBufferSize = 0;

	free(mBuffer);
	mBuffer = NULL;
}

void ByteParser::GetSize(int rAlloc)
{
	if(mCount + rAlloc >= mBufferSize) {
		AllocMoreMemory(rAlloc < 512 ? 512 : rAlloc);
	}
}

void ByteParser::AllocMoreMemory(int rSize)  
{
	mBufferSize += rSize;
	byte* buffer = (byte*)malloc(mBufferSize);

	if(mBuffer != NULL) {
		memcpy(buffer, mBuffer, mBufferSize - rSize);
		free(mBuffer);
	}
	mBuffer = buffer;
}