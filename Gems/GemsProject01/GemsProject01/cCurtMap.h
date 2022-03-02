#pragma once
#include "cCurtArray.h"
#include <string>
template <class T>
class cCurtMap {
public:
	cCurtMap();
	cCurtMap(unsigned int _size);
	virtual ~cCurtMap() {};

	T get(std::string key);
	void insert(std::string key, T value);
private:
	unsigned int size = 0;
	static const unsigned int default_size = 10;
	cCurtArray<T> data;
	unsigned int getHashValue(std::string key);
	void allocateSpace();
};