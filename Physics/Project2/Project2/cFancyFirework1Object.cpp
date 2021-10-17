#include "cFancyFirework1Object.h"

cFancyFirework1Object::cFancyFirework1Object() {
}
cFancyFirework1Object::cFancyFirework1Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(1.f, 0.f, 0.f));
}

std::vector<iFireworkObject*> cFancyFirework1Object::triggerStageTwo() {
	std::vector<iFireworkObject*> newFireworks;
	for (int x = 0; x < 2; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), glm::vec3(float(x), 0.f, 0.f), model->vertexColourOverrideHACK));
	}
	return newFireworks;
}