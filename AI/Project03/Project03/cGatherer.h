#pragma once
#include "GathererStates.h"
#include "cMesh.h"
#include <extern/glm/vec3.hpp>

class cGatherer {
public:
	cGatherer(glm::vec3 _position);
	void Update(float deltaTime);
private:
	void UpdateState();
	GathererStates currentState;
	cMesh* mesh;
	glm::vec3 position;
	bool isWaiting;
	float waitMaxTime = 4.f;
	float waitCounter = 0.f;
};