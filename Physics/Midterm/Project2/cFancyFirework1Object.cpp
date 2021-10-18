#include "cFancyFirework1Object.h"

cFancyFirework1Object::cFancyFirework1Object() {
}
cFancyFirework1Object::cFancyFirework1Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(1.f, 0.f, 0.f));
}

std::vector<iFireworkObject*> cFancyFirework1Object::triggerNextStage() {
	std::vector<iFireworkObject*> newFireworks;
	glm::vec3 direction;
	for (int x = 0; x < 40; x++) {
		direction = worldSpace->getRandomVector3(15.f);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}