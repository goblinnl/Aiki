#ifndef LIST_H
#define LIST_H

#include <vector>

template<class T>
class List : public std::vector<T> {
public:
	int mCount;

public:
	List();
	List(const List<T>& rCopy);
	List(const std::vector<T>& rCopy);

	void Clear();
	void Insert(int rIndex, const T& rObject);
	void Set(int rIndex, const T& rObject);
	void Reserve(int rIndex);

	bool Contains(const T& rObject);

	int Add(const T& rObject);

	T& Buffer();
	T& Get(int rIndex);
	T RemoveAdd(int rIndex);
};
#endif // LIST_H