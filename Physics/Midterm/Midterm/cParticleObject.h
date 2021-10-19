#pragma once
#include "src/graphics/cModel.h"
#include <physics/cParticle.h>

#include "sParticleTemplate.h"

class cParticleObject {
public:
	cParticleObject();
	cParticleObject(nPhysics::cParticle* _particle, cModel* _model);
	cParticleObject(sParticleTemplate* _particleTemp, cModel* _model, glm::vec3 direction, glm::vec3 position);
	nPhysics::cParticle* particle;
	cModel* model;
	bool isAtDistanceLimit();
	bool isAtTimeLimit();
private:
	glm::vec3 origin;
	float age;
	float timeLimit;
	float distanceLimit;
};