#pragma once
#include "cCurtArray.h"
#include <string>
template <class T, class T2>
class cCurtMap {
public:
	cCurtMap();
	cCurtMap(unsigned int _size);
	virtual ~cCurtMap() {};

	T2 get(T key);
	void insert(T key, T2 value);
	T2 operator[](T key);
private:
	unsigned int size = 0;
	static const unsigned int default_size = 10;
	cCurtArray<T2> data;
	unsigned int getHashValue(T key);
	void allocateSpace();
};