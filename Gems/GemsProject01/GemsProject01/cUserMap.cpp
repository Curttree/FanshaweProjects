#include "cUserMap.h"

cUserMap::cUserMap() {
	this->allocateSpace();
}

cUserMap::cUserMap(LibrarySort sortType) : sortBy(sortType) {
	this->allocateSpace();
}

cUserMap::cUserMap(unsigned int arraySize, unsigned int hashStart, unsigned int hashDenominator, LibrarySort sortType) {
	this->size = arraySize;
	this->startValue = hashStart;
	this->denominator = hashDenominator;
	this->allocateSpace();
	this->sortBy = sortType;
}
cUserMap::~cUserMap() {

}

bool cUserMap::insertAtIndex(unsigned int keyValue, cPerson* song) {
	unsigned int hash = calcHashValue(keyValue);
	if (hash >= data.get_size()) {
		//Our set is too small. Reallocate space and try again.
		unsigned int factor = (hash * 2) / data.get_capacity();
		if (factor == 1) {
			factor++;
		}
		reallocateSpace(factor);
		insertAtIndex(keyValue, song);
	}
	else {
		cCurtLinkedList <cPair<unsigned int, cPerson*>>* result = data[hash];
		if (result->get_size() > 0 && result->getCurrent().First != keyValue) {
			// The existing data in this bucket does not match the ID we were expecting. We have a collision.
			if (denominator > 1) {
				unsigned int new_denominator = denominator / 10;
				if (new_denominator == denominator) {
					denominator -= 1;
				}
				else {
					denominator = new_denominator;
				}
				reallocateSpace(2);
				//Recurse, decreasing denominator until we are either using a 1 to 1 array, or a denominator low enough that there is not a collision.
				insertAtIndex(keyValue, song);
			}
			else {
				//Something went wrong. Return false so we can display an appropriate error.
				return false;
			}
		}
		else {
			if (result->get_size() == 0 || sortBy == LibrarySort::UNSORTED) {
				//Either inserting into an empty list, or we don't care about the sort order.
				result->insert(cPair<unsigned int, cPerson*>(keyValue, song));
			}
			else {
				switch (sortBy) {
				
				default:
					// Some other sorting we didn't plan for. Just insert.
					result->insert(cPair<unsigned int, cPerson*>(keyValue, song));
					break;
				}
			}
		}
	}

	return true;
}

cCurtLinkedList <cPair<unsigned int, cPerson*>>* cUserMap::getAtIndex(unsigned int keyValue) {
	unsigned int hash = calcHashValue(keyValue);
	if (hash <= data.get_size()) {
		return data[hash];
	}
	//The requested data would be outside of the scope of our map.
	return 0;
}

void cUserMap::allocateSpace() {
	for (unsigned int count = 0; count < this->size; count++) {
		cCurtLinkedList<cPair<unsigned int, cPerson*>>* newArray = new cCurtLinkedList <cPair<unsigned int, cPerson*>>();
		this->data.push_back(newArray);
	}
}

void cUserMap::reallocateSpace(unsigned int multiplier) {
	cCurtArray<cCurtLinkedList<cPair<unsigned int, cPerson*>>*>* newData = new cCurtArray<cCurtLinkedList<cPair<unsigned int, cPerson*>>*>(data.get_capacity() * multiplier);
	this->size = newData->get_capacity();
	for (unsigned int count = 0; count <= this->size; count++) {
		cCurtLinkedList<cPair<unsigned int, cPerson*>>* newArray = new cCurtLinkedList <cPair<unsigned int, cPerson*>>();
		newData->push_back(newArray);
	}
	//Now populate with old data.
	for (unsigned int count = 0; count < data.get_size(); count++) {
		if (!data[count]->isEmpty() && data[count]->current != 0) {
			//There's data to copy over.
			if (data[count]->getCurrent().Second != 0) {
				newData->setAtIndex(calcHashValue(data[count]->getCurrent().First), data[count]);
			}
		}
		else {
			//delete data[count];
		}

	}
	data = *newData;
}

unsigned int cUserMap::calcHashValue(unsigned int keyValue) {
	//Simple hash function. Could be optimized to limit conflicts, however since we are mostly using for IDs, there should really only be conflicts if our hash size is too small.
	//If we want to offer better performance and cannot decrease our increment for IDs, we could also swap this for a better optimized hash function.
	return (keyValue - this->startValue) / denominator;
}

cPerson* cUserMap::findPerson(std::string first, std::string last) {
	// Could store in a tree if we want a performance increase here. Right now just do basic linear search.
	for (unsigned int count = 0; count < data.get_size(); count++) {
		if (data[count]->current->data.Second->first == first && data[count]->current->data.Second->last == last) {
			return data[count]->current->data.Second;
		}
	}
	return 0;
}

bool cUserMap::buildOutputArray(cPerson*& output, unsigned int& length) {
	cCurtArray<cPerson*> result;
	if (data.get_size() == 0) {
		length = 0;
		output = new cPerson[0];
		return false;
	}

	for (unsigned int x = 0; x < data.get_size(); x++) {
		if (data[x]->get_size() > 0 && data[x]->current !=nullptr) {
			//There is a user.
			result.push_back(data[x]->current->data.Second);
		}
	}
	length = result.get_size();
	cPerson* copyOfResult = new cPerson[length];

	for (unsigned int index = 0; index < length; index++) {
		copyOfResult[index] = *result[index];
	}

	output = copyOfResult;
	return true;
}