#include "cFireworkBuilder.h"
#include "cFireworkFactory.h"
#include "cFuseFactory.h"
#include <functional>

cFireworkBuilder* cFireworkBuilder::_instance = 0;
cFireworkBuilder::cFireworkBuilder() {

}

cFireworkBuilder* cFireworkBuilder::Instance() {
	if (_instance == 0) {
		_instance = new cFireworkBuilder;
	}

	return _instance;
}
iFireworkObject* cFireworkBuilder::buildFirework(int fireworkNum, glm::vec3 position, glm::vec3 velocity = glm::vec3(0.f)) {
	iFireworkObject* result;
	switch (fireworkNum) {
	case(0):
		result = cFireworkFactory::Instance()->createFireworkObject(0, position, velocity);
		result->fuse = cFuseFactory::Instance()->createFuse(2, &result->particle->hitApex, 120.f);
		break;
	case(1):
		result = cFireworkFactory::Instance()->createFireworkObject(1, position, velocity);
		result->fuse = cFuseFactory::Instance()->createFuse(1, &result->particle->hitApex, 0.f);
		break;
	case(2):
		result = cFireworkFactory::Instance()->createFireworkObject(2, position, velocity);
		result->fuse = cFuseFactory::Instance()->createFuse(1, &result->particle->hitApex, 0.f);
		break;
	default:
		// Invalid selection. Build a firwork that will fizzle out immediately.
		result = cFireworkFactory::Instance()->createFireworkObject(0,position,velocity);
		result->fuse = cFuseFactory::Instance()->createFuse(2, &result->particle->hitApex, 0.f);
		break;
	}

	return result;
}