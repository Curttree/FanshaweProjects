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
iFireworkObject* cFireworkBuilder::buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 direction, glm::vec3 debrisColour) {
	iFireworkObject* result;
	switch (fireworkNum) {
	case(0):
		result = cFireworkFactory::Instance()->createFireworkObject(0, position, direction * 1.f, debrisColour);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 120.f);
		break;
	case(1):
		result = cFireworkFactory::Instance()->createFireworkObject(1, position, direction * 150.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 45.f);
		break;
	case(2):
		result = cFireworkFactory::Instance()->createFireworkObject(2, position, direction * 40.f);
		result->fuse = cFuseFactory::Instance()->createFuse(APEX_FUSE, &result->particle->hitApex, 0.f);
		break;
	case(3):
		result = cFireworkFactory::Instance()->createFireworkObject(3, position, direction * 120.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 40.f);
		break;
	case(4):
		result = cFireworkFactory::Instance()->createFireworkObject(4, position, direction * 20.f);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 60.f);
		break;
	default:
		// Invalid selection. Build a firwork that will fizzle out immediately.
		result = cFireworkFactory::Instance()->createFireworkObject(0,position,direction);
		result->fuse = cFuseFactory::Instance()->createFuse(TIMED_FUSE, &result->particle->hitApex, 0.f);
		break;
	}

	return result;
}