#pragma once
#include "cMesh.h"
#include <extern/glm/gtc/matrix_transform.hpp>

class cDungeonFactory {
public:
	static cDungeonFactory* Instance();
	cMesh* createDungeonPiece(int type, glm::vec3 position, glm::vec3 orientation = glm::vec3(0.f));
private:
	cDungeonFactory();
	static cDungeonFactory* _instance;
};