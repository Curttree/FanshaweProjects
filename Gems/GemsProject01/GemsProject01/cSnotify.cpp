#include "cSnotify.h"

cSnotify::cSnotify() {

}

cSnotify::~cSnotify() {

}

// This returns a COPY of the users library, in the form of a regular dynamic array.
bool cSnotify::GetUsersSongLibrary(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary)
{
	// The caller would do something like this (assume the user ID = 28472382)
	//
	//	cSong* pTheSongs = 0;
	//	unsigned int arraySize = 0;
	//	GetUsersSongLibrary( 28472382, pTheSongs, arraySize );
	// 
	// Inside this method, you'd do something like this:

	// TODO: Find that user... 

	// Alloate a heap based array to hold all the songs...

//	sizeOfLibary = WhateverYouHaveToDoToGetThisValue();
//	pCopyOfLibrary = new cSong[sizeOfLibary];

	// The array and the size of the array are "returned" by reference to the caller. 

	// TODO: Copy all the songs over

	return true;
}

// All these return true if they work, false if not (for any reason)
// You can add some error information to the by referenece error string, if you want. 
bool cSnotify::AddUser(cPerson* pPerson, std::string& errorString) {
	//Adds new user and inserts key into song library map (doing here so less error checking)
	return true;
}
// This will overwrite the data contained in user 
// NOTE: It "finds" the user by matching BOTH the "unique Snofity ID" AND the "SIN" 
//        (Social Insurance Number)
// So if either of these don't match (or aren't found) then it reutrns false. 
bool cSnotify::UpdateUser(cPerson* pPerson, std::string& errorString) {

	cPerson* candidate = FindUserBySnotifyID(pPerson->getSnotifyUniqueUserID());
	// Also verify SIN.
	if (candidate == 0 || candidate->SIN != pPerson->SIN) {
		return false;
	}
	// TODO: Update the user.
	return true;
}

bool cSnotify::DeleteUser(unsigned int SnotifyUserID, std::string& errorString) {
	//Remove from vector and song library map.
	return true;
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	cSong* alreadyExists = FindSong(pSong->getUniqueID());
	if (alreadyExists == 0) {
		songList.insertAtIndex(pSong->getUniqueID(), pSong);
		return true;
	}
	else {
		errorString = "ERROR: Cannot add song, it already exists in our library.";
		return false;
	}
}

// This finds a match, based on the "unique ID", and overwrites the data. 
bool cSnotify::UpdateSong(cSong* pSong, std::string& errorString) {
	cSong* toUpdate = FindSong(pSong->getUniqueID());
	if (toUpdate != 0) {
		toUpdate = pSong;
		return true;
	}
	//We mustn't have been able to find the song.
	errorString = "ERROR: Song does not exist in library.";
	return false;
}

// ************************************************************************
// NOTE: Deleting the song deletes it from ALL users that have this song!!!
// ************************************************************************
bool cSnotify::DeleteSong(unsigned int UniqueSongID, std::string& errorString) {

	cCurtLinkedList <cPair<unsigned int, cSong*>>* result = songList.getAtIndex(UniqueSongID);
	if (result != 0 && result->get_size() > 0) {
		result->empty();
		return true;
	}
	//We mustn't have been able to find the song.
	errorString = "ERROR: Song does not exist in library.";
	return false;
}

// This associates a particular song with a particular user. 
// (returns true if the song is already there)
bool cSnotify::AddSongToUserLibrary(unsigned int snotifyUserID, cSong* pNewSong, std::string& errorString) {
	// Add to vector of users song, insert/update record in song user's map.
	return true;
}

bool cSnotify::RemoveSongFromUserLibrary(unsigned int snotifyUserID, unsigned int SnotifySongID, std::string& errorString) {
	// Remove from vector of users song, delete record in song user's map.
	return true;
}

bool cSnotify::UpdateRatingOnSong(unsigned int SnotifyUserID, unsigned int songUniqueID, unsigned int newRating) {
	// Find song and then update song rating.
	return true;
}

// Returns 0, NULL, or nullptr if you can't find the song.
// (Can return optional information on errorString)
// 
// ****************************************************************************
// NOTE: This updates the "numberOfTimesPlayed" in THIS USER'S playlist-library
// ****************************************************************************
cSong* cSnotify::GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString) {
	return 0;
}

// Note that the songRating is "returned" by reference. 
// If it can't find a match (i.e. returns false), then the rating isn't valid. 
bool cSnotify::GetCurrentSongRating(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& songRating) {
	std::string error;
	cSong* song = GetSong(snotifyUserID, songUniqueID, error);
	if (song == 0) {
		return false;
	}
	songRating = song->rating;
	return true;
}

bool cSnotify::GetCurrentSongNumberOfPlays(unsigned int snotifyUserID, unsigned int songUniqueID, unsigned int& numberOfPlays) {
	std::string error;
	cSong* song = GetSong(snotifyUserID, songUniqueID, error);
	if (song == 0) {
		return false;
	}
	numberOfPlays = song->numberOfTimesPlayed;
	return true;
}

// Returns 0, NULL, or nullptr if these fail
cPerson* cSnotify::FindUserBySIN(unsigned int SIN) {
	return 0;
}

cPerson* cSnotify::FindUserBySnotifyID(unsigned int SnotifyID) {
	return 0;
}

// See notes about passing by pointer.
// The TLDR is that if I update what this is pointed to, IT'S UPDATED ACROSS THE ENTIRE SYSTEM
// (You can also assume that with duplicates, it will return the 1st one it finds. I don't care which.)
cSong* cSnotify::FindSong(std::string title, std::string artist) {
	//Slower because we are not indexing by song. Iterate through the list instead.

	return 0;
}

cSong* cSnotify::FindSong(unsigned int uniqueID) {
	cCurtLinkedList <cPair<unsigned int, cSong*>>* result = songList.getAtIndex(uniqueID);
	if (result->get_size() > 0)
	{
		return result->current->data.Second;
	}
	return 0;
}

// Same, but array is sorted by song title in ascending order ("A to Z")
bool cSnotify::GetUsersSongLibraryAscendingByTitle(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	// Build a results set then sort
	return true;
}

// Same, but array is sorted by artist name, ascending ("A to Z")
bool cSnotify::GetUsersSongLibraryAscendingByArtist(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	// Build a results set then sort
	return true;
}


bool cSnotify::GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	return true;
}

// Sorded ascending by Snotify ID
bool cSnotify::GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	return true;
}

// These should all be sorted ascending ("A to Z") IF there are duplicates
bool cSnotify::FindUsersFirstName(std::string firstName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	return true;
}

bool cSnotify::FindUsersLastName(std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	return true;
}

// Sorted ascending by last name THEN first name.
bool cSnotify::FindUsersFirstLastNames(std::string firstName, std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	return true;
}