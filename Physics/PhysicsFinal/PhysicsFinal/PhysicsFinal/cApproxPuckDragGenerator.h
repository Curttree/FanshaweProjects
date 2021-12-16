#pragma once
#include <Physics/iForceGenerator.h>

class cApproxPuckDragGenerator : public iForceGenerator {
public:
	cApproxPuckDragGenerator(const glm::vec3& _dragAcceleration);
	virtual ~cApproxPuckDragGenerator();
	virtual void Update(cParticle* particle, float deltaTime);
private:
	glm::vec3 approxPuckDragAcceleration;
};