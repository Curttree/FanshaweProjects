#pragma once
#include "cMesh.h"

class cBase {
public:
	cBase(glm::vec3 _position);
	virtual ~cBase();
private:
	cMesh* mesh;
	glm::vec3 position;
};