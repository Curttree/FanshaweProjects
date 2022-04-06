#include "cSong.h"

cSong::cSong()
{
	this->rating = 0;
	this->numberOfTimesPlayed = 0;

	// Use (basically) the same approach as generation of User IDs.
	this->uniqueID = m_NEXT_Snotify_UniqueSongID;
	// Increment for next created user by a small random amount 
	const unsigned int MAX_ID_INCREEMNT = 11;
	this->m_NEXT_Snotify_UniqueSongID += (rand() % MAX_ID_INCREEMNT);
}

unsigned int cSong::m_NEXT_Snotify_UniqueSongID = 0;