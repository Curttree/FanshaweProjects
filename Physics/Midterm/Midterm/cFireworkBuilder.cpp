#include "cFireworkBuilder.h"
#include "cFireworkFactory.h"
#include "cFuseFactory.h"
#include <functional>

// Define fuses.
#define APEX_FUSE 1
#define TIMED_FUSE 2

cFireworkBuilder* cFireworkBuilder::_instance = 0;
cFireworkBuilder::cFireworkBuilder() {

}

cFireworkBuilder* cFireworkBuilder::Instance() {
	if (_instance == 0) {
		_instance = new cFireworkBuilder;
	}

	return _instance;
}
iFireworkObject* cFireworkBuilder::buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 debrisDirection, glm::vec3 debrisColour) {
	iFireworkObject* result;
	switch (fireworkNum) {
	case(0):
		result = cFireworkFactory::Instance()->createFireworkObject(0, position, debrisDirection * 1.f, debrisColour);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 120.f);
		break;
	case(1):
		result = cFireworkFactory::Instance()->createFireworkObject(1, position, determineDirection(-1.f,1.f) * 150.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 40.f);
		break;
	case(2):
		result = cFireworkFactory::Instance()->createFireworkObject(2, position, determineDirection(-3.f, 3.f) * 40.f);
		result->fuse = cFuseFactory::Instance()->createFuse(APEX_FUSE, &result->particle->hitApex, 0.f);
		break;
	case(3):
		result = cFireworkFactory::Instance()->createFireworkObject(3, position, determineDirection(-2.f, 2.f) * 120.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 30.f);
		break;
	case(4):
		result = cFireworkFactory::Instance()->createFireworkObject(4, position, debrisDirection * 20.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 60.f);
		break;
	default:
		// Invalid selection. Build a firwork that will fizzle out immediately.
		result = cFireworkFactory::Instance()->createFireworkObject(0,position, debrisDirection);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 0.f);
		break;
	}

	return result;
}

glm::vec3 cFireworkBuilder::determineDirection(float lowerBound, float upperBound) {
	glm::vec3 result = (worldSpace->axes[0] * mathHelper->getRandom(lowerBound, upperBound)) + (worldSpace->axes[1] * 5.f) + (worldSpace->axes[2] * mathHelper->getRandom(lowerBound, upperBound));
	result = glm::normalize(result);
	return result;
}