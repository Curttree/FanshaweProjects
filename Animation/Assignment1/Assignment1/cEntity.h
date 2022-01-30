#pragma once
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/gtx/quaternion.hpp>


#include "cMesh.h"
#include "Animation.h"

class cEntity {
public:
	cEntity();
	virtual ~cEntity();

	void TimeStep(float deltaTime);

	cMesh* mesh;

	// Use values directly rather than using the component approach. Talk to Feeney to see if he will accept the component approach for his projects.
	glm::vec3 position;
	glm::quat rotation;
	float scale;	// Only use uniform scale for now until there is a need for non-uniform scaling.

	Animation animation;

};