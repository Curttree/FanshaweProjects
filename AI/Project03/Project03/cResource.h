#pragma once
#include "cMesh.h"

class cResource {
public:
	cResource(glm::vec3 _position);
	virtual ~cResource();
private:
	cMesh* mesh;
	glm::vec3 position;
};