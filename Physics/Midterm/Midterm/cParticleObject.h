#pragma once
#include "src/graphics/cModel.h"

#include "sParticleTemplate.h"
#include "cWorldSpace.h"
#include "cParticle.h"

class cParticleObject {
public:
	cParticleObject();
	cParticleObject(cParticle* _particle, cModel* _model);
	cParticleObject(sParticleTemplate* _particleTemp, cModel* _model, glm::vec3 direction, glm::vec3 position);
	cParticle* particle;
	cModel* model;
	bool isAtDistanceLimit();
	bool isAtTimeLimit(); 
	bool isAboveGround();
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
private:
	glm::vec3 origin;
	float age;
	float timeLimit;
	float distanceLimit;
};