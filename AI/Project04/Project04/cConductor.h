#pragma once
#include <vector>
#include "cMesh.h"

class cConductor {
public:
	void AddObject(cMesh* obj);
	bool CheckForCollisions(cMesh* object, cMesh* ignore, cMesh*& other, float& penetration);
private:
	std::vector<cMesh*> objects;
};