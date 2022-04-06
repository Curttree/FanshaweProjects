// GemsProject01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cPersonGenerator.h"
#include "cMusicGenerator.h"
#include "cCurtMap.h"
#include "cCurtLinkedList.h"
#include "cExistenceMap.h"
#include "cSongLibraryMap.h"
#include "cSnotify.h"

int main()
{
	std::string errorString;
	cPersonGenerator personGenerator;
	personGenerator.LoadCensusFiles("baby.csv","surname.csv","Street_Names.csv",errorString);

	cMusicGenerator music;
	music.LoadMusicInformation("hot_stuff_2.csv", errorString);
	cSong* test = music.findSong("Sign Of The Times", "Harry Styles");
	if (test != 0) {
		std::cout << "Found song with ID: " << test->getUniqueID() << std::endl;
	}
	else {
		std::cout << "Couldn't find song." << std::endl;
	}

	cPerson* person;
	for (unsigned int test = 0; test < 10; test++) {
		person = personGenerator.generateRandomPerson();
		std::cout << "(" << person->getGenderAsString() << ") " << person->first << " " << person->middle << " " << person->last << std::endl;
	}

	//cSongLibraryMap testMap;
	//testMap.insertAtIndex(test->getUniqueID(), test);
	//cCurtLinkedList <cPair<unsigned int, cSong*>>* test2 = testMap.getAtIndex(test->getUniqueID());
	//std::cout << test2->current->data.Second->name << std::endl;
	cSnotify snotify;
	snotify.AddSong(test, errorString);
	snotify.DeleteSong(test->getUniqueID(), errorString);
	if (errorString.empty()) {
		std::cout << "All operations completed successfully!" << std::endl;
	}
	else {
		std::cout << errorString << std::endl;
	}
}