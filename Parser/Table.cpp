#include "Table.h"

Table::Table(ArgParser* rParser) : mCount(0), mBufferSize(0), mKeys(NULL), mValues(NULL)
{
	AllocateMoreMemory();
}

Table::~Table()
{
	Clear();
	free(mKeys);
	free(mValues);
}

void Table::SetIndex(int rIndex, Variable* rVariable)
{
	
}

void Table::Set(const MCString& rKey, Variable* rVariable)
{

}

int Table::GetHasKeyIndex(const MCString& rKey)
{

}

bool Table::HasKey(const MCString& rKey)
{

}

int Table::GetNextIndex(int rIndex)
{

}

Variable* Table::Get(const MCString& rKey)
{

}

MCString Table::GetKeyByIndex(int rIndex)
{

}

void Table::Delete(const MCString& rKey)
{

}

void Table::Clear()
{

}

Variable* Table::GetByIndex(int rIndex)
{

}

Variable* Table::operator[](const MCString& rKey)
{

}

Variable* Table::operator[](int rIndex)
{

}

void Table::AllocateMoreMemory()
{

}
