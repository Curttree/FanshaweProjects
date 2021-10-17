#include "cFancyFirework4Object.h"

cFancyFirework4Object::cFancyFirework4Object() {
}
cFancyFirework4Object::cFancyFirework4Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(0.94f, 0.9f, .55f));
}

std::vector<iFireworkObject*> cFancyFirework4Object::triggerNextStage() {
	std::vector<iFireworkObject*> newFireworks;
	float multiplier = 1.f;
	for (int x = 0; x < 10; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), glm::vec3(float(x)/2.f * multiplier, 0.1f, float(x)), model->vertexColourOverrideHACK));
		multiplier *= -1.f;
	}
	return newFireworks;
}