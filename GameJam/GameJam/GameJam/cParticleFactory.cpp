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
		result = ::g_pGameEngine->entityManager.CreateParticle("Bubble.bmp", -1.f, position, followCameraPosition, glm::vec3(10.f));
		break;
	}
	case PARTICLE_BUBBLE: {
		break;
	}
	default:
		break;
	}
	return result;
}