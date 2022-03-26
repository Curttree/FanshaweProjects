#pragma once
#include "cMesh.h"
#include "cGatherer.h"

class cResource {
public:
	cResource(glm::vec3 _position);
	virtual ~cResource();
	bool isActive = true;
private:
	cMesh* mesh;
	glm::vec3 position;
	cGatherer* collectedBy;
};