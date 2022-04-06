#pragma once
#include "cCurtArray.h"
#include <string>
template <class T>
class cCurtMap {
public:
	virtual ~cCurtMap() {};
private:
	unsigned int size = 0;
	static const unsigned int default_size = 10;
	cCurtArray<T*> data;

	unsigned int  getHashValue(std::string key) {
		// TODO: Replace with own hash function.
		// Add up all the characters and return that number
		// unsigned int is 32 bits (assume that an int is 32 and not 64 bits, like a "long long")

		unsigned int hashValue = 0;

		for (unsigned int index = 0; index != key.length(); index++)
		{
			// Can index a string like an array
			unsigned char curChar = key[index];

			hashValue += (unsigned int)curChar;
		}

		// This is likely way larger than the size of my smart array, so I'll adjust it
		hashValue %= this->size;

		return hashValue;
	}

	void allocateSpace() {
		for (unsigned int index = 0; index != this->size; index++) {
			T* entry = new T();
			this->data.push_back(entry);
		}
	}
public:

	cCurtMap() {
		size = default_size;
		allocateSpace();
	}

	cCurtMap(unsigned int _size) {
		size = _size;
		allocateSpace();
	}

	T get(std::string key) {
		//unsigned int hash = this->getHashValue(key);
		//return this->data.getAtIndex(hash);
		return 0;
	}

	void insert(std::string key, T value) {
		//unsigned int hash = this->getHashValue(key);
		////TODO: Check to see if someone already exists and handle safely.
		//this->data.setAtIndex(hash, &value);
	}

};