#include "List.h"


template<class T>
List<T>::List() : mCount(0), std::vector<T>()
{

}

template<class T>
List<T>::List(const List<T>& rCopy) : std::vector<T>(rCopy), mCount(rCopy.mCount)
{

}

template<class T>
List<T>::List(const std::vector<T>& rCopy) : std::vector<T>(rCopy), mCount((int)rCopy.size())
{

}

template<class T>
void List<T>::Clear()
{
	Clear();
	mCount = 0;
}

template<class T>
void List<T>::Insert(int rIndex, const T& rObject)
{
	Insert(begin() + rIndex, rObject);
	mCount++;
}

template<class T>
void List<T>::Set(int rIndex, const T& rObject)
{
	(*this)[rIndex] = rObject;
}

template<class T>
void List<T>::Reserve(int rIndex)
{
	Reserve(rIndex);
}

template<class T>
bool List<T>::Contains(const T& rObject)
{
	for(int i = 0; i < mCount; i++) {
		if((*this)[i] == rObject) {
			return true;
		}
	}
	return false;
}

template<class T>
int List<T>::Add(const T& rObject)
{
	push_back(rObject);
	return mCount++;
}

template<class T>
T& List<T>::Buffer()
{
	return &(*this)[0];
}

template<class T>
T& List<T>::Get(int rIndex)
{
	return (*this)[rIndex];
}

template<class T>
T List<T>::RemoveAt(int rIndex)
{
	T object = (*this)[rIndex];

	erase(begin() + rIndex);
	mCount--;

	return object;
}

template<class T>
int List<T>::GetCount() const
{
	return mCount;
}

template<class T>
void List<T>::SetCount(const int& rCount)
{
	mCount = rCount;
}