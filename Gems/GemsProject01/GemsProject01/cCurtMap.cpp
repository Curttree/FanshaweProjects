#include "cCurtMap.h"

template <class T>
cCurtMap<class T>::cCurtMap() {
	size = default_size;
	allocateSpace();
}

template <class T>
cCurtMap<class T>::cCurtMap(unsigned int _size) {
	size = _size;
	allocateSpace();
}

template <class T>
T cCurtMap<class T>::get(std::string key) {
	unsigned int hash = this->getHashValue(key);
	return this->data.getAtIndex(hash);
}

template <class T>
void cCurtMap<class T>::insert(std::string key, T value) {
	unsigned int hash = this->getHashValue(key);
	//TODO: Check to see if someone already exists and handle safely.
	this->data.setAtIndex(hash);
}

template <class T>
unsigned int  cCurtMap<class T>::getHashValue(std::string key) {
	// TODO: Replace with own hash function.
	// Add up all the characters and return that number
	// unsigned int is 32 bits (assume that an int is 32 and not 64 bits, like a "long long")

	unsigned int hashValue = 0;

	for (unsigned int index = 0; index != keyValue.length(); index++)
	{
		// Can index a string like an array
		unsigned char curChar = keyValue[index];

		hashValue += (unsigned int)curChar;
	}

	// This is likely way larger than the size of my smart array, so I'll adjust it
	hashValue %= this->currentSize;

	return hashValue;
}

template <class T>
void cCurtMap<class T>::allocateSpace() {
	for (unsigned int index = 0; index != this->size; index++) {
		T entry;
		this->data.push_back(entry);
	}
}