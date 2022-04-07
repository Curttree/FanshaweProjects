#pragma once
#include "cCurtArray.h"
#include "cCurtLinkedList.h"
#include "cPerson.h"
#include "cPair.h"
#include "eLibrarySort.h"

class cUserMap {
public:
	cUserMap();
	cUserMap(LibrarySort sortType);
	cUserMap(unsigned int arraySize, unsigned int hashStart, unsigned int hashDenominator, LibrarySort sortType = LibrarySort::UNSORTED);
	virtual ~cUserMap();
	bool insertAtIndex(unsigned int keyValue, cPerson* song);
	cCurtLinkedList <cPair<unsigned int, cPerson*>>* getAtIndex(unsigned int keyValue);
	cPerson* findPerson(std::string first, std::string last);
	bool buildOutputArray(cPerson*& output, unsigned int& length);

private:
	void allocateSpace();
	void reallocateSpace(unsigned int multiplier);
	unsigned int calcHashValue(unsigned int keyValue);
	static const unsigned int INITIAL_SIZE = 1024;
	unsigned int size = INITIAL_SIZE;
	unsigned int denominator = 1;
	unsigned int startValue = 0;
	cCurtArray<cCurtLinkedList<cPair<unsigned int, cPerson*>>*> data;
	LibrarySort sortBy = LibrarySort::UNSORTED;
};