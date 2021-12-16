#pragma once
#include "cEntity.h"

// Creates different specifications of particles.
class cParticleFactory {
public:
	static cParticleFactory* Instance();
	virtual ~cParticleFactory();
	cEntity* createParticle(int projNumber, glm::vec3 position, glm::vec3 velocity = glm::vec3(0.f));
private:
	cParticleFactory();
	static cParticleFactory* _instance;
};