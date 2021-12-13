#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <extern/glm/vec4.hpp> // glm::vec4

class cDungeonReader {
public:
	bool ReadDungeonFromTSV(std::string fileName);
	void GetDungeonSize(int& x_size, int& z_size);
	int GetObjectAtLocation(int x_coordinate, int z_coordinate);
	//Returns a vec4 representation of which walls to place ( top, bottom, left, right)
	glm::vec4 DetermineWallOrientation(int x_coordinate, int z_coordinate);
private:
	std::vector<std::string> dungeon;
};