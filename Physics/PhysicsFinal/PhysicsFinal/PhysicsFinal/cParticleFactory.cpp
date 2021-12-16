#include "cParticleFactory.h"
#include "cPuck.h"

#define PUCK 0
#define IRON 1
#define NYLON 2


cMathHelper* _mathHelper = cMathHelper::Instance();

cParticleFactory* cParticleFactory::_instance = 0;
cParticleFactory::cParticleFactory() {

}

cParticleFactory::~cParticleFactory() {
}

cEntity* cParticleFactory::createParticle(int material, glm::vec3 position, glm::vec3 velocity) {
	cEntity* result = 0;

	switch (material) {
	case(PUCK):
		result = new cPuck(position, velocity);
		break;
	case(IRON):
		break;
	case(NYLON):
		break;
	default:
		break;
	}
	return result;

}

cParticleFactory* cParticleFactory::Instance() {
	if (_instance == 0) {
		_instance = new cParticleFactory;
	}

	return _instance;
}