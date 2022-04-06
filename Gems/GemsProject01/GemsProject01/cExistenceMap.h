#pragma once
#include <limits.h>
#include "cCurtArray.h"
#include <string>
// A container with many 'buckets' that just tracks a bool value. Meant for temporarily verifying existance of a string in a dataset.
// Can upsize from unsigned int to long long, or add option to reallocate space if we need more buckets.
class cExistenceMap {
public:
	cExistenceMap();
	virtual ~cExistenceMap();
	void insertAtIndex(std::string keyValue);
	bool getAtIndex(std::string keyValue);
private:
	void allocateSpace();
	unsigned int calcHashValue(std::string keyValue);
	static const unsigned int INITIAL_SIZE = 1000003;		//Large prime number.
	unsigned int size = INITIAL_SIZE;
	cCurtArray<bool> data;
};