#pragma once
#include "cDungeonReader.h"
#include "cMesh.h"

class cDungeonMaster {
public:
	cDungeonMaster();
	~cDungeonMaster();
	void BuildDungeonFromTSV(std::string fileName);
	std::vector<cMesh*> dungeonModels;
private:
	cDungeonReader* dungeonReader;
	void PlaceWalls(glm::vec3 position, glm::vec4 wallInfo); 
	void SetupDoors(cMesh* door, int x, int z);
};