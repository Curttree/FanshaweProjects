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
	glm::vec3 direction;
	for (int x = 0; x < 20; x++) {
		direction = worldSpace->getRandomVector3(5.f, true, glm::pi<float>() / 3.f);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}