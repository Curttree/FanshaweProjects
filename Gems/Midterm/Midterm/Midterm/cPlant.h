#pragma once
#include "cMesh.h"
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3

class cPlant {
public:
	cPlant();
	cPlant(glm::vec3 location, float _maturityTime);
	virtual ~cPlant();
	void TimeStep(float deltaTime);
	void HasBeenEaten();
	glm::vec3 position;
	bool hasBloomed;
	float maturityTime = 0.f;
	float elapsedTime;
private:
	float mature_size = 0.2f;
	cMesh* mesh;
};