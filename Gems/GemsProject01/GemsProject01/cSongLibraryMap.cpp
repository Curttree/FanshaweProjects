#include "cSongLibraryMap.h"

cSongLibraryMap::cSongLibraryMap() {
	this->allocateSpace();
}

cSongLibraryMap::cSongLibraryMap(LibrarySort sortType) : sortBy(sortType) {
	this->allocateSpace();
}

cSongLibraryMap::cSongLibraryMap(unsigned int arraySize, unsigned int hashStart, unsigned int hashDenominator, LibrarySort sortType) {
	this->size = arraySize;
	this->startValue = hashStart;
	this->denominator = hashDenominator;
	this->allocateSpace();
	this->sortBy = sortType;
}
cSongLibraryMap::~cSongLibraryMap() {

}

bool cSongLibraryMap::insertAtIndex(unsigned int keyValue, cSong* song) {
	unsigned int hash = calcHashValue(keyValue);
	if (hash >= data.get_size()) {
		//Our set is too small. Reallocate space and try again.
		unsigned int factor = (hash * 2) / data.get_capacity();
		reallocateSpace(factor);
		insertAtIndex(keyValue, song);
	}
	else {
		cCurtLinkedList <cPair<unsigned int, cSong*>>* result = data[hash];
		if (result->get_size() > 0 && result->getCurrent().First != keyValue) {
			// The existing data in this bucket does not match the ID we were expecting. We have a collision.
			if (denominator > 1) {
				denominator -= 1;
				reallocateSpace(1);
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
				result->insert(cPair<unsigned int, cSong*>(keyValue, song));
			}
			else {
				switch (sortBy) {
				case LibrarySort::ARTIST: {
					std::string newValue = song->artist;
					bool found = false;
					result->moveToHead();
					do {
						if (newValue.compare(result->current->data.Second->artist) <= 0) {
							//We are in the right spot.
							result->insert(cPair<unsigned int, cSong*>(keyValue, song));
							found = true;
						}
						else {
							result->moveForward();
						}
					} while (result->current != result->tail && !found);
					
					if (!found) {
						//We made it to the end and couldn't find an appropriate spot. Insert at tail since it is after everything currently in the list.
						result->insertAtTail(cPair<unsigned int, cSong*>(keyValue, song));
					}
					break;
				}
				case LibrarySort::SONG: {
					std::string newValue = song->name;
					bool found = false;
					result->moveToHead();
					do {
						if (newValue.compare(result->current->data.Second->name) <= 0) {
							//We are in the right spot.
							result->insert(cPair<unsigned int, cSong*>(keyValue, song));
							found = true;
						}
						else {
							result->moveForward();
						}
					} while (result->current != result->tail && !found);

					if (!found) {
						//We made it to the end and couldn't find an appropriate spot. Insert at tail since it is after everything currently in the list.
						result->insertAtTail(cPair<unsigned int, cSong*>(keyValue, song));
					}
					break;
				}
				default:
					// Some other sorting we didn't plan for. Just insert.
					result->insert(cPair<unsigned int, cSong*>(keyValue, song));
					break;
				}
			}
		}
	}

	return true;
}

cCurtLinkedList <cPair<unsigned int, cSong*>>* cSongLibraryMap::getAtIndex(unsigned int keyValue) {
	unsigned int hash = calcHashValue(keyValue);
	if (hash <= data.get_capacity()) {
		return data[hash];
	}
	//The requested data would be outside of the scope of our map.
	return 0;
}

void cSongLibraryMap::allocateSpace() {
	for (unsigned int count = 0; count < this->size; count++) {
		cCurtLinkedList<cPair<unsigned int, cSong*>>* newArray = new cCurtLinkedList <cPair<unsigned int, cSong*>>();
		this->data.push_back(newArray);
	}
}

void cSongLibraryMap::reallocateSpace(unsigned int multiplier) {
	cCurtArray<cCurtLinkedList<cPair<unsigned int, cSong*>>*> newData(data.get_capacity() * multiplier);
	this->size = newData.get_capacity();
	for (unsigned int count = 0; count < this->size; count++) {
		cCurtLinkedList<cPair<unsigned int, cSong*>>* newArray = new cCurtLinkedList <cPair<unsigned int, cSong*>>();
		newData.push_back(newArray);
	}
	//Now populate with old data.
	for (unsigned int count = 0; count < data.get_size(); count++) {
		if (data[count]->get_size() > 0) {
			//There's data to copy over.
			newData.setAtIndex(calcHashValue(data[count]->getCurrent().First), data[count]);
		}
		
	}
	data = newData;
}

unsigned int cSongLibraryMap::calcHashValue(unsigned int keyValue) {
	//Simple hash function. Could be optimized to limit conflicts, however since we are mostly using for IDs, there should really only be conflicts if our hash size is too small.
	//If we want to offer better performance and cannot decrease our increment for IDs, we could also swap this for a better optimized hash function.
	return (keyValue - this->startValue) / denominator;
}

cSong* cSongLibraryMap::findSong(std::string title, std::string artist) {
	// Could store in a tree if we want a performance increase here. Right now just do basic linear search.
	for (unsigned int count = 0; count < data.get_size(); count++) {
		if (data[count]->current->data.Second->artist == artist && data[count]->current->data.Second->name == title) {
			return data[count]->current->data.Second;
		}
	}
	return 0;
}