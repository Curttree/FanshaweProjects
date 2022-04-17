#pragma once
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/gtx/quaternion.hpp>


#include "cMesh.h"
#include "Animation/Animation.h"
#include "Animation/AnimationSequence.h"

class cEntity {
public:
	cEntity();
	virtual ~cEntity();

	virtual void TimeStep(float deltaTime);

	cMesh* mesh;

	// Use values directly rather than using the component approach. Talk to Feeney to see if he will accept the component approach for his projects.
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Animation animation;

	AnimationSequence animations;

};