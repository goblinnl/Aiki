#ifndef MIXER_C_DICTIONARY
#define MIXER_C_DICTIONARY

#include "MCommon.h"
#include "MCArray.h"

template<class TempKey, class TempValue>
class MCDictionary {
private:
	MCArray<TempKey> mKeys;
	MCArray<TempValue> mValues;

public:
	MCDictionary();
	~MCDictionary();

	void Add(const TempKey &rKey, const TempValue &rValue);
	bool HasKey(const TempKey &rKey);
	bool HasValue(const TempValue &rValue);
	int IndexByKey(const TempKey &rKey);
	int IndexByValue(const TempValue &rValue);
	void RemoveByIndex(const int rIndex);
	void RemoveByKey(const TempKey &rKey);
	void RemoveByValue(const TempValue &rValue);

	int Count();

	TempValue& operator[](const TempKey &rKey);
	TempValue& GetValueByIndex(const int rIndex);
	TempKey& GetKeyByIndex(const int rIndex);
};
#endif // MIXER_C_DICTIONARY