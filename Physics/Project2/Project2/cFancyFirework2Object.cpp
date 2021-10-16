#include "cFancyFirework2Object.h"
#include "cFireworkDebrisObject.h"

std::vector<iFireworkObject*> cFancyFirework2Object::triggerStageTwo() {
	std::vector<iFireworkObject*> newFireworks;
	for (int x = 0; x < 5; x++) {
		newFireworks.push_back(fireworkFactory->createFireworkObject(0, particle->GetPosition(), glm::vec3(float(x), float(x), float(x))));
	}
	return newFireworks;
}

bool cFancyFirework2Object::isReadyForStageTwo() {
	if (!particle->isGoingUp()) {
		return true;
	}
	return false;
}