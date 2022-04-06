#include "cPersonGenerator.h"

#include <iostream>
#include <fstream>
#include <sstream>		// String Stream
#include <string>

cPersonGenerator::cPersonGenerator() {

}

cPersonGenerator::~cPersonGenerator() {

}

unsigned int cPersonGenerator::getNumberOfNamesLoaded(void) {
	return numNames;
}
unsigned int cPersonGenerator::getNumberOfSurnamesLoaded(void) {
	return numSurnames;
}
unsigned int cPersonGenerator::getNumberOfStreetsLoaded(void) {
	return numStreets;
}

cPerson* cPersonGenerator::generateRandomPerson(void) {
	if (numNames <= 0 || numSurnames <= 0 || numStreets <= 0) {
		return 0;
	}

	cPerson* result = new cPerson();

	result->gender = (cPerson::eGenderType) (rand() % 4);

	// Technically first and middle could be the same. Nothing wrong with that: https://twitter.com/bardown/status/1475271063584002055
	if ((int)result->gender == 0) {
		//Male
		int index = maleNameCodes.getAtIndex(rand() % maleNameCodes.get_size());
		result->first = babyNames[index]; 
		index = maleNameCodes.getAtIndex(rand() % maleNameCodes.get_size());
		result->middle = babyNames[index];
	}
	else if ((int)result->gender == 1) {
		//Female
		int index = femaleNameCodes.getAtIndex(rand() % femaleNameCodes.get_size());
		result->first = babyNames[index]; 
		index = femaleNameCodes.getAtIndex(rand() % femaleNameCodes.get_size());
		result->middle = babyNames[index];
	}
	else {
		// Non-binary or no information. Select name from entire list.
		result->first = babyNames[rand() % numNames];
		result->middle = babyNames[rand() % numNames];
	}

	result->last = surNames[rand() % numSurnames];

	// We could add a check for uniqueness for SIN number. For now, since the number is quite large and we are handling potential duplicates down the line, I am ommitting uniqeuness validation here,
	// but it could be added later.

	result->SIN = rand() % 1000000000;
	result->age = rand() % 100;

	// We do not have a source for city/province names or their postal codes. We could retrieve one, but it would be another code dependency and isn't used within Snotify app.
	// Since they are not used for anything within our application, do not randomize these, and just use hardcoded values so something is present at least.
	result->city = "London";
	result->province = "Ontario";
	strncpy_s(result->postalCode, "N5Y5R6", 6);

	// TODO: Populate remaining street data.
	result->streetNumber = rand() % 1000;

	//People will be stored in Snotify app. No need to store within the person generator, just return a value.

	return result;
}


bool cPersonGenerator::LoadCensusFiles(
	std::string babyNameFile,
	std::string surnameFile,
	std::string streetNameFile,
	std::string& errorString)
{
	unsigned int result = 0;
	if (!readCSVFile(babyNameFile, numNames, babyNames, errorString)) {
		return false;
	}
	if (!readGenderCodes(babyNameFile, result, maleNameCodes, femaleNameCodes, errorString, 1)) {
		return false;
	}
	if (!readCSVFile(surnameFile, numSurnames, surNames, errorString)) {
		return false;
	}
	if (!readCSVFile(streetNameFile, numStreets, streetNames, errorString)) {
		return false;
	}

	return true;
}


// Here's a simple way to load the comma delimited files:
bool cPersonGenerator::readCSVFile(std::string fileName, unsigned int& numberLoaded, cCurtArray<std::string>& output, std::string& errorString, unsigned int tokenOfInterest)
{
	// Open the file
	std::ifstream namesFile(fileName);
	if (!namesFile.is_open())
	{
		errorString = "Didn't open file";
		std::cout << errorString << std::endl;
		return false;
	}

	// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
	// SMITH,1,2442977,828.19,828.19,70.9,23.11,0.5,0.89,2.19,2.4
	//
	// - rank is how popular the last name is, like 1st, 2nd, etc.
	// - count is how many people have that last name, so 2,442,977 people have last name "Smith"
	// - prop100k is the ratio per 100,000 people. Smith is 828.19, 
	//            meaning that there's a 828.19 out of 100,000 chance (0.82819% chance)
	//            that someone is named "Smith"

	std::string theLine;

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == tokenOfInterest)
			{
				//std::cout << token << std::endl;
				output.push_back(token);
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	numberLoaded = lineCount;

	return true;
}

// Example for reading files. Will not be calling, but keeping around as a reference.
bool cPersonGenerator::readCSVFile(void)
{
	// Open the file
	std::ifstream namesFile("Names_2010Census.csv");
	if (!namesFile.is_open())
	{
		std::cout << "Didn't open file" << std::endl;
		return false;
	}

	// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
	// SMITH,1,2442977,828.19,828.19,70.9,23.11,0.5,0.89,2.19,2.4
	//
	// - rank is how popular the last name is, like 1st, 2nd, etc.
	// - count is how many people have that last name, so 2,442,977 people have last name "Smith"
	// - prop100k is the ratio per 100,000 people. Smith is 828.19, 
	//            meaning that there's a 828.19 out of 100,000 chance (0.82819% chance)
	//            that someone is named "Smith"

	std::string theLine;

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == 0)
			{
				std::cout << token << std::endl;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	return true;
}

bool cPersonGenerator::readGenderCodes(std::string fileName, unsigned int& numberLoaded, cCurtArray<int>& outputMale, cCurtArray<int>& outputFemale, std::string& errorString, unsigned int tokenOfInterest) {
	std::ifstream namesFile(fileName);
	if (!namesFile.is_open())
	{
		errorString = "Didn't open file";
		std::cout << errorString << std::endl;
		return false;
	}

	std::string theLine;

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == tokenOfInterest)
			{
				if (token == "M" || token == "m") {
					outputMale.push_back(lineCount-1);
				}
				if (token == "F" || token == "f") {
					outputFemale.push_back(lineCount-1);
				}
				//TODO: determine if we should handle other genders.
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}
	numberLoaded = lineCount;

	return true;
}