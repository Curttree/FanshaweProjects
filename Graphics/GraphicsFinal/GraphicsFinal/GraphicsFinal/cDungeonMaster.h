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
	void PlaceCrystals();
	void PlacePlants();
	void PlaceTorches(int x_size, int z_size);
	void CreateFlame(glm::vec3 position);
	int candlesPlaced = 8;		// Add 8 as first 0-8 are are reserved for other lights
};