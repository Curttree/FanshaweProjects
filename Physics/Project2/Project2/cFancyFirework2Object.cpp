#include "cFancyFirework2Object.h"

cFancyFirework2Object::cFancyFirework2Object() {
}
cFancyFirework2Object::cFancyFirework2Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(0.f, 1.f, 0.f));
}

std::vector<iFireworkObject*> cFancyFirework2Object::triggerNextStage() {
	std::vector<iFireworkObject*> newFireworks;
	for (int x = 0; x < 5; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), glm::vec3(float(x), float(x), float(x)), model->vertexColourOverrideHACK));
	}
	return newFireworks;
}