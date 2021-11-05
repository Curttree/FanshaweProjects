#pragma once
#include <string>
#include <vector>
#include <extern/glm/vec3.hpp>


class cMazeManager {
public:
	bool LoadMazeFromFile(std::string fileName);
	bool ValidatePosition(glm::vec3 position, glm::vec3 heading);
private:
	std::vector<std::string> maze;
};