#include "cFancyFirework3Object.h"

cFancyFirework3Object::cFancyFirework3Object() {
}
cFancyFirework3Object::cFancyFirework3Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(0.94f, 0.9f, .55f));
}

std::vector<iFireworkObject*> cFancyFirework3Object::triggerNextStage() {
	std::vector<iFireworkObject*> newFireworks;
	float multiplier = 1.5f;
	for (int x = 0; x < 2; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(4, particle->GetPosition(), glm::vec3(multiplier, 0.5f, float(x)), model->vertexColourOverrideHACK));
		multiplier *= -1.f;
	}
	return newFireworks;
}