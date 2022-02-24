#pragma once
#include "iForceGenerator.h"

class cGravityGenerator : public iForceGenerator {
public:
	cGravityGenerator(const glm::vec3& _gravityAcceleration);
	virtual ~cGravityGenerator();
	virtual void Update(cParticle* particle, float deltaTime);
private:
	glm::vec3 gravityAcceleration;
};