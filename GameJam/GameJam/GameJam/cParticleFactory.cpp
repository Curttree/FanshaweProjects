#include "cParticleFactory.h"
#include "particleDefs.h"
#include "globals.h"


cParticleFactory* cParticleFactory::_instance = 0;
cParticleFactory::cParticleFactory() {

}

cParticleFactory* cParticleFactory::Instance() {
	if (_instance == 0) {
		_instance = new cParticleFactory;
	}

	return _instance;
}

cParticle* cParticleFactory::createParticle(int type, glm::vec3 position, bool followCameraPosition, glm::vec3 velocity) {
	cParticle* result = 0;
	switch (type) {
	case PARTICLE_TEST: {
		result = ::g_pGameEngine->entityManager.CreateParticle("bubble.bmp", -1.f, position, 0.2f, followCameraPosition, glm::vec3(10.f));
		break;
	}
	case PARTICLE_BUBBLE: {
		result = ::g_pGameEngine->entityManager.CreateParticle("bubble.bmp", 3.f, position, 0.2f, followCameraPosition, glm::vec3(0.5f));
		result->velocity = glm::vec3(0.f, 0.1f, 0.f);
		break;
	}
	case PARTICLE_SMOKE: {
		result = ::g_pGameEngine->entityManager.CreateParticle("Animations/smoke00.bmp", 0.5f, position, 0.2f, followCameraPosition, glm::vec3(1.5f));
		result->textures.push_back("Animations/smoke00.bmp");
		result->textures.push_back("Animations/smoke01.bmp");
		result->textures.push_back("Animations/smoke02.bmp");
		result->textures.push_back("Animations/smoke03.bmp");
		result->textures.push_back("Animations/smoke04.bmp");
	}
	default:
		break;
	}
	return result;
}