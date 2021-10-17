#include "cFancyFirework1Object.h"
#include "cFireworkDebrisObject.h"

std::vector<iFireworkObject*> cFancyFirework1Object::triggerStageTwo() {
	std::vector<iFireworkObject*> newFireworks;
	for (int x = 0; x < 2; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), glm::vec3(float(x), 0.f, 0.f)));
	}
	return newFireworks;
}