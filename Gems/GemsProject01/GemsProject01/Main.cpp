// GemsProject01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "cPersonGenerator.h"
#include "cMusicGenerator.h"
#include "cCurtLinkedList.h"
#include "cExistenceMap.h"
#include "cSongLibraryMap.h"
#include "cSnotify.h"

int main()
{
	srand(time(NULL));
	std::string errorString;
	cPersonGenerator personGenerator;
	personGenerator.LoadCensusFiles("yob1992.csv","Names_2010Census.csv","Street_Names.csv",errorString);

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
	person = personGenerator.generateRandomPerson();

	cSnotify snotify;
	snotify.AddSong(test, errorString);
	snotify.AddSongToUserLibrary(person->getSnotifyUniqueUserID(), test, errorString);

	for (unsigned int counter = 0; counter < 25; counter++) {
		cSong* test2 = music.getRandomSong();
		snotify.AddSong(test2, errorString);
		snotify.AddSongToUserLibrary(person->getSnotifyUniqueUserID(), test2, errorString);
	}

	cSong* libraryResult;
	unsigned int libraryLength;
	snotify.GetUsersSongLibraryAscendingByTitle(person->getSnotifyUniqueUserID(), libraryResult, libraryLength);

	std::cout << "Library length of " << libraryLength << "|| Songs are: ";
	for (unsigned int x = 0; x < libraryLength; x++) {
		std::cout << libraryResult[x].artist << " - " << libraryResult[x].name << std::endl;
	}
	std::cout << std::endl;

	cSong* libraryResult2;
	unsigned int libraryLength2;
	snotify.GetUsersSongLibraryAscendingByArtist(person->getSnotifyUniqueUserID(), libraryResult2, libraryLength2);

	std::cout << "Library by artist length of " << libraryLength2 << "|| Artists are: ";
	for (unsigned int x = 0; x < libraryLength2; x++) {
		std::cout << libraryResult2[x].artist << " - " << libraryResult2[x].name << std::endl;
	}
	std::cout << std::endl;

	cPerson* person2;
	for (unsigned int test = 0; test < 10; test++) {
		person2 = personGenerator.generateRandomPerson();
		snotify.AddUser(person2,errorString);
	}

	cPerson* sinSearch = snotify.FindUserBySnotifyID(person2->getSnotifyUniqueUserID());
	std::cout << "Searched for user with ID of " << person2->SIN << " | Found: " << sinSearch->last << " , " << sinSearch->first << std::endl;

	cPerson* firstList;
	unsigned int firstLength;
	snotify.FindUsersLastName(person2->last, firstList, firstLength);
	std::cout << "Users with the last name of " << person2->last << ": ";
	for (unsigned int x = 0; x < firstLength; x++) {
		std::cout << firstList[x].last << " , " << firstList[x].first << " (" << firstList[x].SIN << ")" << std::endl;
	}
	std::cout << std::endl;

	cPerson* userList;
	unsigned int userLength;
	snotify.GetUsers(userList, userLength);
	std::cout << "User list length of " << userLength << "|| Users are: ";
	for (unsigned int x = 0; x < userLength; x++) {
		std::cout << userList[x].last << " , " << userList[x].first  << " (" << userList[x].SIN << ")" << std::endl;
	}
	std::cout << std::endl;

	snotify.AddSongToUserLibrary(userList[0].getSnotifyUniqueUserID(), &libraryResult[5], errorString);
	snotify.AddSongToUserLibrary(userList[1].getSnotifyUniqueUserID(), &libraryResult[5], errorString);
	snotify.AddSongToUserLibrary(userList[1].getSnotifyUniqueUserID(), &libraryResult[6], errorString);
	snotify.AddSongToUserLibrary(userList[2].getSnotifyUniqueUserID(), &libraryResult[5], errorString);

	snotify.UpdateRatingOnSong(userList[0].getSnotifyUniqueUserID(), libraryResult[5].getUniqueID(), 5);
	snotify.UpdateRatingOnSong(userList[1].getSnotifyUniqueUserID(), libraryResult[5].getUniqueID(), 4);

	unsigned int rating;
	snotify.GetCurrentSongRating(userList[0].getSnotifyUniqueUserID(), libraryResult[5].getUniqueID(), rating);

	std::cout << userList[0].first << " " << userList[0].last << " rated " << libraryResult[5].name << " " << rating << std::endl;

	snotify.GetCurrentSongRating(userList[1].getSnotifyUniqueUserID(), libraryResult[5].getUniqueID(), rating);

	std::cout << userList[1].first << " " << userList[1].last << " rated " << libraryResult[5].name << " " << rating << std::endl;

	snotify.GetCurrentSongRating(userList[2].getSnotifyUniqueUserID(), libraryResult[5].getUniqueID(), rating);

	std::cout << userList[2].first << " " << userList[2].last << " rated " << libraryResult[5].name << " " << rating << std::endl;

	cSong* librarySecond;
	unsigned int librarySecondLength;
	snotify.GetUsersSongLibrary(userList[1].getSnotifyUniqueUserID(), librarySecond, librarySecondLength);
	std::cout << "Library for " << userList[1].first <<" "<< userList[1].last << "|| Songs are: ";
	for (unsigned int x = 0; x < librarySecondLength; x++) {
		std::cout << librarySecond[x].artist << " - " << librarySecond[x].name << std::endl;
	}

	snotify.DeleteSong(libraryResult[5].getUniqueID(), errorString);

	snotify.GetUsersSongLibrary(userList[1].getSnotifyUniqueUserID(), librarySecond, librarySecondLength);
	std::cout << "Deleted a song. Library for " << userList[1].first << " " << userList[1].last << "|| Songs are: ";
	for (unsigned int x = 0; x < librarySecondLength; x++) {
		std::cout << librarySecond[x].artist << " - " << librarySecond[x].name << std::endl;
	}

	snotify.DeleteUser(userList[1].getSnotifyUniqueUserID(), errorString);
	snotify.GetUsers(userList, userLength);
	std::cout << "Deleted someone. User list length of " << userLength << "|| Users are: ";
	for (unsigned int x = 0; x < userLength; x++) {
		std::cout << userList[x].last << " , " << userList[x].first << " (" << userList[x].SIN << ")" << std::endl;
	}
	std::cout << std::endl;

	if (errorString.empty()) {
		std::cout << "All operations completed successfully!" << std::endl;
	}
	else {
		std::cout << errorString << std::endl;
	}
}