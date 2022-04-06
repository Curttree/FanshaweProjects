#include "cMusicGenerator.h"

#include <iostream>
#include <fstream>
#include <sstream>		// String Stream
#include <string>


cMusicGenerator::cMusicGenerator() {

}
cMusicGenerator::~cMusicGenerator() {

}

bool cMusicGenerator::LoadMusicInformation(std::string musicFileName, std::string& errorString) {
	// Open the file
	std::ifstream musicFile(musicFileName);
	if (!musicFile.is_open())
	{
		errorString = "Didn't open file";
		std::cout << errorString << std::endl;
		return false;
	}

	std::string theLine;

	unsigned int lineCount = 0;
	unsigned int dupes = 0;
	cExistenceMap loadedSongs;
	while (std::getline(musicFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		std::string name;
		std::string artist;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == 3)
			{
				name = token;
			}
			if (tokenCount == 4)
			{
				artist = token;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
		std::string testKey =artist + "_" + name;
		if (!loadedSongs.getAtIndex(testKey)) {
			loadedSongs.insertAtIndex(testKey);
			cSong* song = new cSong();
			song->artist = artist;
			song->name = name;
			songs.push_back(song);
		}
		else {
			dupes++;
		}

	}

	std::cout << "Lines read = " << lineCount << std::endl;
	std::cout << "Duplicates avoided = " << dupes << std::endl;

	return true;
}

cSong* cMusicGenerator::getRandomSong(void) {
	return songs[rand() % songs.get_size()];
}

// Returns 0, NULL, or nullptr if no song is found
// This must be an EXACT MATCH for what's in the billboard100 file. 
// So case sensitive, etc. 
cSong* cMusicGenerator::findSong(std::string songName, std::string artist) {
	//Slow. Could optimize with map of data or red black tree for quicker access if we think we will be calling regularly/song library continues to grow.
	for (unsigned int counter = 0; counter < songs.get_size(); counter++) {
		if (songs[counter]->artist == artist && songs[counter]->name == songName) {
			return songs[counter];
		}
	}
	//Traversed all values in the array and did not find a match.
	return 0;
}