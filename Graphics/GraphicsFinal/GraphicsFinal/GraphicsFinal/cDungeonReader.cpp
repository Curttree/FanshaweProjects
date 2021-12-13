#include "cDungeonReader.h"


bool cDungeonReader::ReadDungeonFromTSV(std::string fileName) {
	std::stringstream ss;
	ss << fileName;
	std::ifstream inputFile(ss.str());

	if (inputFile.fail()) {
		// Something is wrong. We could not load the maze;
		std::cout << "Could not read the dungeon. Unexpected behavior could occur." << std::endl;
		return false;
	}

	std::string line;
	std::size_t colonValue;
	while (std::getline(inputFile, line)) {
		dungeon.push_back(line);
	}

	return true;
}

void cDungeonReader::GetDungeonSize(int& x_size, int& z_size) {
	if (dungeon.size() == 0) {
		x_size = 0;
		z_size = 0;
		return;
	}

	x_size = dungeon.size();
	z_size = dungeon[0].size();
	return;
}
int cDungeonReader::GetObjectAtLocation(int x_coordinate, int z_coordinate) {
	if (dungeon[x_coordinate][z_coordinate] == 'F') {
		return 1;
	}
	else if (dungeon[x_coordinate][z_coordinate] == '-') {
		// Walls will be rendered using helper function. Do not read.
		return -1;
	}
	else if (dungeon[x_coordinate][z_coordinate] == 'D') {
		// Door.
		return 3;
	}

	// Unknown character detected.
	return -1;
}

glm::vec4 cDungeonReader::DetermineWallOrientation(int x_coordinate, int z_coordinate){
	glm::vec4 result = glm::vec4(0.f);
	//Check right
	if (x_coordinate <= 0 || dungeon[x_coordinate - 1][z_coordinate] == '-') {
			// Wall needs to be placed on top.
			result.x = 1.f;
	}
	//Check left
	if (x_coordinate >= dungeon.size() - 1 || dungeon[x_coordinate + 1][z_coordinate] == '-') {
		// Wall needs to be placed on bottom.
		result.y = 1.f;
	}
	//Check left
	if (z_coordinate <= 0 || dungeon[x_coordinate][z_coordinate - 1] == '-') {
		// Wall needs to be placed on left.
		result.z = 1.f;
	}
	//Check bottom
	if (z_coordinate >= dungeon[0].size() - 1 || dungeon[x_coordinate][z_coordinate+1] == '-') {
		// Wall needs to be placed on bottom.
		result.w = 1.f;
	}

	return result;
}