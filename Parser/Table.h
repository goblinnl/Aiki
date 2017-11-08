#ifndef TABLE_H
#define TABLE_H

#include "Variable.h"

class Table {
public:
	int mCount;

private:
	MCString** mKeys;
	Variable** mValues;
	int mBufferSize;

public:
	Table();
	~Table();	

	void SetIndex(int rIndex, Variable* rVariable);
	void Set(const MCString& rKey, Variable* rVariable);

	int GetHasKeyIndex(const MCString& rKey);
	bool HasKey(const MCString& rKey);

	int GetNextIndex(int rIndex);

	Variable* Get(const MCString& rKey);
	MCString GetKeyByIndex(int rIndex);

	void Delete(const MCString& rKey);
	void Clear();

	Variable* GetByIndex(int rIndex);
	Variable* operator[](const MCString& rKey);
	Variable* operator[](int rIndex);

private:
	void AllocateMoreMemory();
};

#endif // TABLE_H