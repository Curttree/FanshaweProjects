#include "cCurtMap.h"

template <class T, class T2>
cCurtMap<T, T2>::cCurtMap() {
	size = default_size;
	allocateSpace();
}

template <class T, class T2>
cCurtMap<T, T2>::cCurtMap(unsigned int _size) {
	size = _size;
	allocateSpace();
}

template <class T, class T2>
T2 cCurtMap<T, T2>::get(T key) {
	unsigned int hash = this->getHashValue(key);
	return this->data.getAtIndex(hash);
}

template <class T, class T2>
T2 cCurtMap<T, T2>::operator[](T key) {
	return this->get(key);
}

template <class T, class T2>
void cCurtMap<T, T2>::insert(T key, T2 value) {
	unsigned int hash = this->getHashValue(key);
	//TODO: Check to see if someone already exists and handle safely.
	this->data.setAtIndex(hash);
}

template <class T, class T2>
unsigned int  cCurtMap<T, T2>::getHashValue(T key) {
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
	hashValue %= this->currentSize;

	return hashValue;
}

template <class T, class T2>
void cCurtMap<T, T2>::allocateSpace() {
	for (unsigned int index = 0; index != this->size; index++) {
		T entry;
		this->data.push_back(entry);
	}
}