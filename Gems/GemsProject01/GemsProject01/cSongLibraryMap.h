#pragma once
#include "cCurtArray.h"
#include "cCurtLinkedList.h"
#include "cSong.h"
#include "cPair.h"
#include "eLibrarySort.h"

class cSongLibraryMap {
public:
	cSongLibraryMap();
	cSongLibraryMap(LibrarySort sortType);
	cSongLibraryMap(unsigned int arraySize, unsigned int hashStart, unsigned int hashDenominator, LibrarySort sortType = LibrarySort::UNSORTED);
	virtual ~cSongLibraryMap();
	bool insertAtIndex(unsigned int keyValue, cSong* song);
	cCurtLinkedList <cPair<unsigned int, cSong*>>* getAtIndex(unsigned int keyValue);
	cSong* findSong(std::string title, std::string artist);

private:
	void allocateSpace();
	void reallocateSpace(unsigned int multiplier);
	unsigned int calcHashValue(unsigned int keyValue);
	static const unsigned int INITIAL_SIZE = 1024;
	unsigned int size = INITIAL_SIZE;
	unsigned int denominator = 1;
	unsigned int startValue = 0;
	cCurtArray<cCurtLinkedList<cPair<unsigned int, cSong*>>*> data;
	LibrarySort sortBy = LibrarySort::UNSORTED;
};