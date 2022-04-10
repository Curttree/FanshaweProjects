#include "cSnotify.h"

cSnotify::cSnotify() : userSongLibrary(100024, 10000000, 1), userSongLibraryByArtist(100024, 10000000, 1, LibrarySort::ARTIST), userSongLibraryByName(100024, 10000000, 1, LibrarySort::SONG), userList(100024, 10000000, 1), userListBySIN(100000, 0, 100) {
	// Potential issue if someone updates the cPerson starting value without changing this. Dangerous if this were production code. Would add tighter coupling if adapting this beyond this assignment.
}

cSnotify::~cSnotify() {

}

// This returns a COPY of the users library, in the form of a regular dynamic array.
bool cSnotify::GetUsersSongLibrary(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary)
{
	cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibrary.getAtIndex(snotifyUserID);
	if (usersLibrary == 0) {
		//We don't have an entry for that user. Return false.
		sizeOfLibary = 0;
		pLibraryArray = new cSong[0];
		return false;
	}
	sizeOfLibary = usersLibrary->get_size();
	cSong* copyOfLibrary = new cSong[sizeOfLibary];
	usersLibrary->moveToHead();
	for (unsigned int index = 0; index < sizeOfLibary; index++) {
		copyOfLibrary[index] = *usersLibrary->current->data.Second;
		usersLibrary->moveForward();
	}
	pLibraryArray = copyOfLibrary;
	return true;
}

// All these return true if they work, false if not (for any reason)
// You can add some error information to the by referenece error string, if you want. 
bool cSnotify::AddUser(cPerson* pPerson, std::string& errorString) {
	//Adds new user and inserts key into song library map (doing here so less error checking)
	cPerson* alreadyExists = FindUserBySnotifyID(pPerson->getSnotifyUniqueUserID());
	if (alreadyExists == 0) {
		userList.insertAtIndex(pPerson->getSnotifyUniqueUserID(), pPerson);
		userListBySIN.insertAtIndex(pPerson->SIN, pPerson);
	}
	else if (alreadyExists->SIN == pPerson->SIN){
		errorString = "ERROR: Cannot add person, they already exist..though I am not sure you should have access to that info.";
		return false;
	}
	else {
		errorString = "ERROR: Duplicate IDs with different SINs detected.";
		return false;
	}

	//Note: In practice, we could make this async if these lists are only going to be internal. Could also improve performance by swapping list with RB tree.
	InsertIntoFirstNameSortedList(pPerson);
	InsertIntoLastNameSortedList(pPerson);

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
		errorString = "ERROR: Cannot find matching person. Verify UserID and SIN.";
		return false;
	}
	//Remove the old candidate from the first and last lists.

	RemoveFromFirstNameSortedList(candidate);
	RemoveFromLastNameSortedList(candidate);

	candidate = pPerson;

	//Update place in first and last lists.
	InsertIntoFirstNameSortedList(candidate);
	InsertIntoLastNameSortedList(candidate);
	return true;
}

bool cSnotify::DeleteUser(unsigned int SnotifyUserID, std::string& errorString) {
	//Remove from the user lists but don't delete the pointer..	People still exist even if they are not users.
	cCurtLinkedList <cPair<unsigned int, cPerson*>>* result = userList.getAtIndex(SnotifyUserID);
	if (result != 0 && result->get_size() > 0) {
		RemoveFromFirstNameSortedList(result->current->data.Second);
		RemoveFromLastNameSortedList(result->current->data.Second);

		cCurtLinkedList <cPair<unsigned int, cPerson*>>* resultSIN = userListBySIN.getAtIndex(result->current->data.Second->SIN);
		if (resultSIN != 0 && resultSIN->get_size() > 0) {
			resultSIN->empty();
		}

		result->empty();

		//Also empty their song library.
		cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibrary.getAtIndex(SnotifyUserID);
		if (usersLibrary->get_size() > 0) {
			usersLibrary->moveToHead();
			while (usersLibrary->current != usersLibrary->tail) {
				cCurtLinkedList <cPair<unsigned int, cPerson*>>* userSongs = songsByUser.getAtIndex(usersLibrary->getCurrent().Second->getUniqueID());
				if (usersLibrary->get_size() == 0) {
					// Library is empty.
					return 0;
				}

				userSongs->moveToHead();
				while (userSongs->current != userSongs->tail && userSongs->current->data.Second->getSnotifyUniqueUserID() != SnotifyUserID) {
					userSongs->moveForward();
				}
				// We are either at the tail, or have found our result. Double check which one.
				if (userSongs->current->data.Second->getSnotifyUniqueUserID() == SnotifyUserID) {
					userSongs->deleteNode();
				}
				usersLibrary->moveForward();
			}
			//Also handle tail.
			{
				cCurtLinkedList <cPair<unsigned int, cPerson*>>* userSongs = songsByUser.getAtIndex(usersLibrary->getCurrent().Second->getUniqueID());
				if (usersLibrary->get_size() == 0) {
					// Library is empty.
					return 0;
				}

				userSongs->moveToHead();
				while (userSongs->current != userSongs->tail && userSongs->current->data.Second->getSnotifyUniqueUserID() != SnotifyUserID) {
					userSongs->moveForward();
				}
				// We are either at the tail, or have found our result. Double check which one.
				if (userSongs->current->data.Second->getSnotifyUniqueUserID() == SnotifyUserID) {
					userSongs->deleteNode();
				}
			}

		}

		//Now that the song mappings are clear, we can empty the list.
		usersLibrary->empty();

		return true;
	}
	//We mustn't have been able to find the person.
	errorString = "ERROR: User does not exist in Snotify.";
	return false;
}

bool cSnotify::AddSong(cSong* pSong, std::string& errorString) {
	cSong* alreadyExists = FindSong(pSong->getUniqueID());
	if (alreadyExists == 0) {
		songList.insertAtIndex(pSong->getUniqueID(), pSong);
		return true;
	}
	else {
		errorString = "ERROR: Cannot add song, it already exists in Snotify.";
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
	errorString = "ERROR: Song does not exist in Snotify.";
	return false;
}

// ************************************************************************
// NOTE: Deleting the song deletes it from ALL users that have this song!!!
// ************************************************************************
bool cSnotify::DeleteSong(unsigned int UniqueSongID, std::string& errorString) {
	cCurtLinkedList <cPair<unsigned int, cSong*>>* result = songList.getAtIndex(UniqueSongID);
	bool found = false;
	if (result != 0 && result->get_size() > 0) {
		cCurtLinkedList <cPair<unsigned int, cPerson*>>* usersWithSong = songsByUser.getAtIndex(UniqueSongID); 
		if (usersWithSong->get_size() > 0) {
			usersWithSong->moveToHead();
			while (usersWithSong->current != usersWithSong->tail) {
				cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibrary.getAtIndex(usersWithSong->getCurrent().Second->getSnotifyUniqueUserID());
				if (usersLibrary->get_size() == 0) {
					// Library is empty.
					return 0;
				}

				usersLibrary->moveToHead();
				while (usersLibrary->current != usersLibrary->tail && usersLibrary->current->data.Second->getUniqueID() != UniqueSongID) {
					usersLibrary->moveForward();
				}
				// We are either at the tail, or have found our result. Double check which one.
				if (usersLibrary->current->data.Second->getUniqueID() == UniqueSongID) {
					found = true;
					delete usersLibrary->getCurrent().Second;
					usersLibrary->deleteNode();
				}
				usersWithSong->moveForward();
			}
			//Also handle tail
			if (!found)
			{
				cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibrary.getAtIndex(usersWithSong->getCurrent().Second->getSnotifyUniqueUserID());
				if (usersLibrary->get_size() == 0) {
					// Library is empty.
					return 0;
				}

				usersLibrary->moveToHead();
				while (usersLibrary->current != usersLibrary->tail && usersLibrary->current->data.Second->getUniqueID() != UniqueSongID) {
					usersLibrary->moveForward();
				}
				// We are either at the tail, or have found our result. Double check which one.
				if (usersLibrary->current->data.Second->getUniqueID() == UniqueSongID) {
					delete usersLibrary->getCurrent().Second;
					usersLibrary->deleteNode();
				}
				usersWithSong->moveForward();
			}
		}

		result->empty();
		return true;
	}
	//We mustn't have been able to find the song.
	errorString = "ERROR: Song does not exist in Snotify.";
	return false;
}

// This associates a particular song with a particular user. 
// (returns true if the song is already there)
bool cSnotify::AddSongToUserLibrary(unsigned int snotifyUserID, cSong* pNewSong, std::string& errorString) {
	// Add to vector of users song, insert/update record in song user's map.
	// First check if the song and/or user has been added to Snotify yet.
	cSong* inLibrary = FindSong(pNewSong->getUniqueID());
	if (inLibrary == 0) {
		errorString = "ERROR: Song has not been added to Snotify yet. Make sure to call AddSong before trying to add the song to a user's library.";
		return false;
	}
	cSong* alreadyExists = GetSong(snotifyUserID, pNewSong->getUniqueID(), errorString);
	if (alreadyExists == 0) {
		cPerson* person = FindUserBySnotifyID(snotifyUserID);
		songsByUser.insertAtIndex(pNewSong->getUniqueID(), person);
		//Make a copy of the song so each user can have their own play count and rating.
		cSong* localSong = new cSong(pNewSong->name, pNewSong->artist, pNewSong->getUniqueID());
		userSongLibrary.insertAtIndex(snotifyUserID, localSong);
		userSongLibraryByArtist.insertAtIndex(snotifyUserID, localSong);
		userSongLibraryByName.insertAtIndex(snotifyUserID, localSong);
		return true;
	}
	else {
		//Already exists. Return true as per specs.
		return true;
	}
}

bool cSnotify::RemoveSongFromUserLibrary(unsigned int snotifyUserID, unsigned int SnotifySongID, std::string& errorString) {
	cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibrary.getAtIndex(snotifyUserID);
	if (usersLibrary->get_size() == 0) {
		errorString = "ERROR: User does not have this song in their library.";
		return false;
	}

	usersLibrary->moveToHead();
	while (usersLibrary->current != usersLibrary->tail && usersLibrary->current->data.Second->getUniqueID() != SnotifySongID) {
		usersLibrary->moveForward();
	}

	//Remove from the sorted versions of the library.
	cCurtLinkedList <cPair<unsigned int, cSong*>>* byArtist = userSongLibraryByArtist.getAtIndex(snotifyUserID);
	if (byArtist->get_size() == 0) {
		errorString = "ERROR: User does not have this song in their library.";
		return false;
	}

	byArtist->moveToHead();
	while (byArtist->current != byArtist->tail && byArtist->current->data.Second->getUniqueID() != SnotifySongID) {
		byArtist->moveForward();
	}

	if (byArtist->current->data.Second->getUniqueID() == SnotifySongID) {
		//Don't delete the object, that'll come later. Just remove the node.
		byArtist->deleteNode();
	}

	cCurtLinkedList <cPair<unsigned int, cSong*>>* byName = userSongLibraryByName.getAtIndex(snotifyUserID);
	if (byName->get_size() == 0) {
		errorString = "ERROR: User does not have this song in their library.";
		return false;
	}

	byName->moveToHead();
	while (byName->current != byName->tail && byName->current->data.Second->getUniqueID() != SnotifySongID) {
		byName->moveForward();
	}

	if (byName->current->data.Second->getUniqueID() == SnotifySongID) {
		//Don't delete the object, that'll come later. Just remove the node.
		byName->deleteNode();
	}

	// We have removed nodes from the other libraries. Now handle our initial list.
	// We are either at the tail, or have found our result. Double check which one.
	if (usersLibrary->current->data.Second->getUniqueID() == SnotifySongID) {
		//Since this is a copy, we can delete the object.
		delete usersLibrary->current->data.Second;
		usersLibrary->deleteNode();
	}

	cCurtLinkedList <cPair<unsigned int, cPerson*>>* songUsers = songsByUser.getAtIndex(SnotifySongID);
	songUsers->moveToHead();
	while (songUsers->current != songUsers->tail && songUsers->current->data.Second->getSnotifyUniqueUserID() != snotifyUserID) {
		songUsers->moveForward();
	}
	// We are either at the tail, or have found our result. Double check which one.
	if (songUsers->current->data.Second->getSnotifyUniqueUserID() == snotifyUserID) {
		songUsers->deleteNode();
		return true;
	}

	//If we got to this point, the song must not be in their library.
	errorString = "ERROR: User does not have this song in their library.";
	return false;
}

bool cSnotify::UpdateRatingOnSong(unsigned int SnotifyUserID, unsigned int songUniqueID, unsigned int newRating) {
	// Find song and then update song rating.
	std::string errorString;
	cSong* alreadyExists = GetSong(SnotifyUserID, songUniqueID, errorString);
	if (alreadyExists == 0) {
		errorString = "ERROR: User does not have this song in their library.";
		return false;
	}
	alreadyExists->rating = newRating;
	return true;
}

// Returns 0, NULL, or nullptr if you can't find the song.
// (Can return optional information on errorString)
// 
// ****************************************************************************
// NOTE: This updates the "numberOfTimesPlayed" in THIS USER'S playlist-library
// ****************************************************************************
cSong* cSnotify::GetSong(unsigned int SnotifyUserID, unsigned int songUniqueID, std::string& errorString) {
	cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary =  userSongLibrary.getAtIndex(SnotifyUserID);
	if (usersLibrary->get_size() == 0) {
		// Library is empty.
		return 0;
	}

	usersLibrary->moveToHead();
	while (usersLibrary->current != usersLibrary->tail && usersLibrary->current->data.Second->getUniqueID() != songUniqueID) {
		usersLibrary->moveForward();
	}
	// We are either at the tail, or have found our result. Double check which one.
	if (usersLibrary->current->data.Second->getUniqueID() == songUniqueID) {
		usersLibrary->current->data.Second->numberOfTimesPlayed++;
		return usersLibrary->current->data.Second;
	}
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
	cCurtLinkedList <cPair<unsigned int, cPerson*>>* result = userListBySIN.getAtIndex(SIN);
	if (result != 0 && result->get_size() > 0)
	{
		return result->current->data.Second;
	}
	return 0;
}

cPerson* cSnotify::FindUserBySnotifyID(unsigned int SnotifyID) {
	cCurtLinkedList <cPair<unsigned int, cPerson*>>* result = userList.getAtIndex(SnotifyID);
	if (result != 0 && result->get_size() > 0)
	{
		return result->current->data.Second;
	}
	return 0;
}

// See notes about passing by pointer.
// The TLDR is that if I update what this is pointed to, IT'S UPDATED ACROSS THE ENTIRE SYSTEM
// (You can also assume that with duplicates, it will return the 1st one it finds. I don't care which.)
cSong* cSnotify::FindSong(std::string title, std::string artist) {
	//Slower because we are not indexing by song. Iterate through the list instead. Could index by title/artist or store song name/id pairs in a tree if we wanted a performance increase here.
	return songList.findSong(title, artist);
}

cSong* cSnotify::FindSong(unsigned int uniqueID) {
	cCurtLinkedList <cPair<unsigned int, cSong*>>* result = songList.getAtIndex(uniqueID);
	if (result != 0 && result->get_size() > 0)
	{
		return result->current->data.Second;
	}
	return 0;
}

// Same, but array is sorted by song title in ascending order ("A to Z")
bool cSnotify::GetUsersSongLibraryAscendingByTitle(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	// Build a results set then sort
	cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibraryByName.getAtIndex(snotifyUserID);
	if (usersLibrary == 0) {
		//We don't have an entry for that user. Return false.
		sizeOfLibary = 0;
		pLibraryArray = new cSong[0];
		return false;
	}
	sizeOfLibary = usersLibrary->get_size();
	cSong* copyOfLibrary = new cSong[sizeOfLibary];
	usersLibrary->moveToHead();
	for (unsigned int index = 0; index < sizeOfLibary; index++) {
		copyOfLibrary[index] = *usersLibrary->current->data.Second;
		usersLibrary->moveForward();
	}
	pLibraryArray = copyOfLibrary;
	return true;
}

// Same, but array is sorted by artist name, ascending ("A to Z")
bool cSnotify::GetUsersSongLibraryAscendingByArtist(unsigned int snotifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary) {
	// Build a results set then sort
	cCurtLinkedList <cPair<unsigned int, cSong*>>* usersLibrary = userSongLibraryByArtist.getAtIndex(snotifyUserID);
	if (usersLibrary == 0) {
		//We don't have an entry for that user. Return false.
		sizeOfLibary = 0;
		pLibraryArray = new cSong[0];
		return false;
	}
	sizeOfLibary = usersLibrary->get_size();
	cSong* copyOfLibrary = new cSong[sizeOfLibary];
	usersLibrary->moveToHead();
	for (unsigned int index = 0; index < sizeOfLibary; index++) {
		copyOfLibrary[index] = *usersLibrary->current->data.Second;
		usersLibrary->moveForward();
	}
	pLibraryArray = copyOfLibrary;
	return true;
}


bool cSnotify::GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a result set. We are pre-sorting, so will be equivalent to GetUsersByID.
	userList.buildOutputArray(pAllTheUsers, sizeOfUserArray);
	return true;
}

// Sorded ascending by Snotify ID
bool cSnotify::GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set.
	userList.buildOutputArray(pAllTheUsers, sizeOfUserArray);
	return true;
}

// These should all be sorted ascending ("A to Z") IF there are duplicates
bool cSnotify::FindUsersFirstName(std::string firstName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	cCurtArray<cPerson*> resultSet;
	bool found = false;
	bool building = false;
	userByFirst.moveToHead();	
	do {
		if (firstName.compare(userByFirst.current->data->first) == 0) {
			//We found a match! Add it to the results set.
			if (!building) {
				building = true;
			}
			resultSet.push_back(userByFirst.current->data);
			userByFirst.moveForward();
		}
		else {
			if (building) {
				//We were building the list, but have encountered someone with a different name. We are done.
				found = true;
			}
			else {
				userByFirst.moveForward();
			}
		}
	} while (userByFirst.current != userByFirst.tail && !found);

	//Double check the tail.
	if (userByFirst.current == userByFirst.tail && firstName.compare(userByFirst.current->data->first) == 0) {
		resultSet.push_back(userByFirst.current->data);
	}
	sizeOfUserArray = resultSet.get_size();
	cPerson* copyOfResult = new cPerson[sizeOfUserArray];

	for (unsigned int index = 0; index < sizeOfUserArray; index++) {
		copyOfResult[index] = *resultSet[index];
	}

	pAllTheUsers = copyOfResult;

	return true;
}

bool cSnotify::FindUsersLastName(std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	cCurtArray<cPerson*> resultSet;
	bool found = false;
	bool building = false;
	userByLast.moveToHead();
	do {
		if (lastName.compare(userByLast.current->data->last) == 0) {
			//We found a match! Add it to the results set.
			if (!building) {
				building = true;
			}
			resultSet.push_back(userByLast.current->data);
			userByLast.moveForward();
		}
		else {
			if (building) {
				//We were building the list, but have encountered someone with a different name. We are done.
				found = true;
			}
			else {
				userByLast.moveForward();
			}
		}
	} while (userByLast.current != userByLast.tail && !found);

	//Double check the tail.
	if (userByLast.current == userByLast.tail && lastName.compare(userByLast.current->data->last) == 0) {
		resultSet.push_back(userByLast.current->data);
	}

	sizeOfUserArray = resultSet.get_size();
	cPerson* copyOfResult = new cPerson[sizeOfUserArray];

	for (unsigned int index = 0; index < sizeOfUserArray; index++) {
		copyOfResult[index] = *resultSet[index];
	}

	pAllTheUsers = copyOfResult;

	return true;
}

// Sorted ascending by last name THEN first name.
bool cSnotify::FindUsersFirstLastNames(std::string firstName, std::string lastName, cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray) {
	// Build a results set then sort
	cCurtArray<cPerson*> resultSet;
	bool found = false;
	bool building = false;
	userByLast.moveToHead();
	do {
		if (lastName.compare(userByLast.current->data->last) == 0 && firstName.compare(userByLast.current->data->first) == 0) {
			//We found a match! Add it to the results set.
			if (!building) {
				building = true;
			}
			resultSet.push_back(userByLast.current->data);
			userByLast.moveForward();
		}
		else {
			if (building) {
				//We were building the list, but have encountered someone with a different name. We are done.
				found = true;
			}
			else {
				userByLast.moveForward();
			}
		}
	} while (userByLast.current != userByLast.tail && !found);

	//Double check the tail.
	if (userByLast.current == userByLast.tail && lastName.compare(userByLast.current->data->last) == 0) {
		resultSet.push_back(userByLast.current->data);
	}

	sizeOfUserArray = resultSet.get_size();
	cPerson* copyOfResult = new cPerson[sizeOfUserArray];

	for (unsigned int index = 0; index < sizeOfUserArray; index++) {
		copyOfResult[index] = *resultSet[index];
	}

	pAllTheUsers = copyOfResult;

	return true;
}

void cSnotify::InsertIntoFirstNameSortedList(cPerson* pPerson) {
	//Do first name.
	std::string newValue = pPerson->first;
	bool found = false;
	userByFirst.moveToHead();
	if (userByFirst.isEmpty()) {
		userByFirst.insert(pPerson);
		return;
	}
	do {
		if (newValue.compare(userByFirst.current->data->first) <= 0) {
			//We are in the right spot. Approx.
			if (newValue.compare(userByFirst.current->data->first) == 0 && newValue.compare(userByFirst.current->data->last) >= 0) {
				//We have the same first name, but by last name, we should be sorted next. Move forward instead.
				userByFirst.moveForward();
			}
			else {
				userByFirst.insert(pPerson);
				found = true;
			}
		}
		else {
			userByFirst.moveForward();
		}
	} while (userByFirst.current != userByFirst.tail && !found);

	if (!found) {
		//We made it to the end and couldn't find an appropriate spot. Insert at tail since it is after everything currently in the list.
		userByFirst.insertAtTail(pPerson);
	}
}

void cSnotify::InsertIntoLastNameSortedList(cPerson* pPerson) {
	//Now do last name (This could go in a helper function).
	std::string newValue = pPerson->last;
	bool found = false;
	userByLast.moveToHead();
	if (userByLast.isEmpty()) {
		userByLast.insert(pPerson);
		return;
	}
	do {
		if (newValue.compare(userByLast.current->data->last) <= 0) {
			//We are in the right spot. Approx.
			if (newValue.compare(userByLast.current->data->last) == 0 && newValue.compare(userByLast.current->data->first) >= 0) {
				//We have the same last name, but by first name, we should be sorted next. Move forward instead.
				userByLast.moveForward();
			}
			else {
				userByLast.insert(pPerson);
				found = true;
			}
		}
		else {
			userByLast.moveForward();
		}
	} while (userByLast.current != userByLast.tail && !found);

	if (!found) {
		//We made it to the end and couldn't find an appropriate spot. Insert at tail since it is after everything currently in the list.
		userByLast.insertAtTail(pPerson);
	}
}
void cSnotify::RemoveFromFirstNameSortedList(cPerson* pPerson) {
	bool found = false;
	userByFirst.moveToHead();
	if (userByFirst.isEmpty()) {
		return;
	}
	do {
		if (userByFirst.current->data->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			//We are in the right spot.
			userByFirst.deleteNode();
		}
		else {
			userByFirst.moveForward();
		}
	} while (userByFirst.current != userByFirst.tail && !found);

	if (!found) {
		//We made it to the end and couldn't find the record. Check to see if it is the tail.
		if (userByFirst.current->data->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			//We are in the right spot.
			userByFirst.deleteNode();
		}
	}
}
void cSnotify::RemoveFromLastNameSortedList(cPerson* pPerson) {
	bool found = false;
	userByLast.moveToHead();
	if (userByLast.isEmpty()) {
		return;
	}
	do {
		if (userByLast.current->data->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			//We are in the right spot.
			userByLast.deleteNode();
		}
		else {
			userByLast.moveForward();
		}
	} while (userByLast.current != userByLast.tail && !found);

	if (!found) {
		//We made it to the end and couldn't find the record. Check to see if it is the tail.
		if (userByLast.current->data->getSnotifyUniqueUserID() == pPerson->getSnotifyUniqueUserID()) {
			//We are in the right spot.
			userByLast.deleteNode();
		}
	}
}