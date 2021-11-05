#include "cMazeManager.h"
#include <sstream>
#include <iostream>
#include <fstream>

bool cMazeManager::LoadMazeFromFile(std::string fileName) {
	std::stringstream ss;
	ss << fileName;
	std::ifstream inputFile(ss.str());

	if (inputFile.fail()) {
		// Something is wrong. We could not load the maze;
		return false;
	}

	std::string line;
	std::size_t colonValue;
	while (std::getline(inputFile, line)) {
		maze.insert(maze.begin(), line);	// Start reading from last x position, reverse row order to accommodate.
	}

	return true;
}

bool cMazeManager::ValidatePosition(glm::vec3 position, glm::vec3 heading) {
	// SNAP POSITION TO GRID	
	int coordX = (int)position.x;
	int coordY = (int)position.y;
	// if moving left
	if (heading.x > 0) {
		coordX++;
	}
	// if moving up
	if (heading.y > 0) {
		coordY++;
	}
	//IF SPACE
	if (maze.at(coordX).at(coordY) == 32) {
		return true;
	}
	//ELSE IF WALL
	else if (maze.at(coordX).at(coordY) == 88) {
		return false;
	}
	else {
		//Some character other than X or space is in the maze. Don't move until we can validate what it is.
		std::cout << "THE MAZE HAS BEEN COMPROMIZED. FOUND " << maze.at(coordX).at(coordY) << std::endl;
		return false;
	}
}