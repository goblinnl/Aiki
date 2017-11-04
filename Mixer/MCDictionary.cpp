#include "MCDictionary.h"


template<class TempKey, class TempValue>
MCDictionary<TempKey, TempValue>::MCDictionary()
{

}

template<class TempKey, class TempValue>
MCDictionary<TempKey, TempValue>::~MCDictionary()
{

}


template<class TempKey, class TempValue>
void MCDictionary<TempKey, TempValue>::Add(const TempKey &rKey, const TempValue &rValue)
{
	if(HasKey(rKey)) {
		return;
	}

	mKeys.Push() = rKey;
	mValues.Push() = rValue;
}


template<class TempKey, class TempValue>
bool MCDictionary<TempKey, TempValue>::HasKey(const TempKey &rKey)
{
	return IndexByKey(rKey) != -1;
}


template<class TempKey, class TempValue>
bool MCDictionary<TempKey, TempValue>::HasValue(const TempValue &rValue)
{
	return IndexByValue(rValue) != -1;
}


template<class TempKey, class TempValue>
int MCDictionary<TempKey, TempValue>::IndexByKey(const TempKey &rKey)
{
	int elements = Count();

	int index = 0;
	for(int i = 0; < elements; i++) {
		index = i;
		if(mKeys[i] == rKey){
			break;
		}	
	}

	if(index < elements) {
		return index;
	}

	return -1;
}


template<class TempKey, class TempValue>
int MCDictionary<TempKey, TempValue>::IndexByValue(const TempValue &rValue)
{
	int elements = Count();

	int index = 0;
	for(int i = 0; i < elements; i++) {
		index = i;
		if(mValues[i] == rValue) {
			break;
		}
	}

	if(index < elements){
		return index;
	}
	return -1;
}

template<class TempKey, class TempValue>
void MCDictionary<TempKey, TempValue>::RemoveByIndex(const int rIndex)
{
	mKeys.PopAt(rIndex);
	mValues.PopAt(rIndex);
}

template<class TempKey, class TempValue>
void MCDictionary<TempKey, TempValue>::RemoveByKey(const TempKey &rKey)
{
	int index = IndexByKey(rKey);
	if(index == -1){
		return;
	}
	RemoveByIndex(index);
}

template<class TempKey, class TempValue>
void MCDictionary<TempKey, TempValue>::RemoveByValue(const TempValue &rValue)
{
	int index = IndexByValue(rValue);
	if(index == -1) {
		return;
	}
	RemoveByIndex(index);
}

template<class TempKey, class TempValue>
int MCDictionary<TempKey, TempValue>::Count()
{
	return mKeys.Count();
}

template<class TempKey, class TempValue>
TempValue& MCDictionary<TempKey, TempValue>::operator[](const TempKey &rKey)
{
	int index = IndexByKey(rKey);

	if(index == -1) {
		mKeys.Push() = rKey;
		return mValues.Push();
	}

	return mValues[index];
}

template<class TempKey, class TempValue>
TempValue& MCDictionary<TempKey, TempValue>::GetValueByIndex(const int rIndex)
{
	return mValues[rIndex];
}

template<class TempKey, class TempValue>
TempKey& MCDictionary<TempKey, TempValue>::GetKeyByIndex(const int rIndex)
{
	return mKeys[rIndex];
}