#include "cExistenceMap.h"

cExistenceMap::cExistenceMap() {
	this->allocateSpace();
}

cExistenceMap::~cExistenceMap() {

}
void cExistenceMap::insertAtIndex(std::string keyValue) {
	unsigned int hash = calcHashValue(keyValue);
	data.setAtIndex(hash, true);
}

bool cExistenceMap::getAtIndex(std::string keyValue) {
	unsigned int hash = calcHashValue(keyValue);
	return data[hash];
}

void cExistenceMap::allocateSpace() {
	for (unsigned int count = 0; count < this->size; count++) {
		this->data.push_back(false);
	}
}
unsigned int cExistenceMap::calcHashValue(std::string keyValue) {
	// Hash function referenced from: http://www.cse.yorku.ca/~oz/hash.html
	// Technically could still have some collisions as we are getting the modulus of our size, but will hopefully be a small amount.
	// Could use a larger size map/more advanced collision handling if we need to ensure 100% accuracy of our read, or are not as concerned about performance.
	// Increasing size x10 only resolves approx. 300 collisions while negatively impacting memory usage.

	unsigned long hash = 5381;
	int c;
	for (unsigned int index = 0; index != keyValue.length(); index++)
	{
		unsigned char curChar = keyValue[index];
		hash = ((hash << 5) + hash) + curChar; /* hash * 33 + c */
	}
	hash %= this->size;
	return hash;
}