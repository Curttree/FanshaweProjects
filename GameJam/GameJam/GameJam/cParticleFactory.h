#pragma once
#include "cParticle.h"
class cParticleFactory {
public:
	static cParticleFactory* Instance();
	cParticle* createParticle(int type, glm::vec3 position, bool followCameraPosition, glm::vec3 velocity = glm::vec3(0.f));
private:
	cParticleFactory();
	static cParticleFactory* _instance;
};