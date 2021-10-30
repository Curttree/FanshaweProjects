#include "cFancyFirework1Object.h"
#include "cFirework_Implementation.h"

cFancyFirework1Object::cFancyFirework1Object(cFirework* _particle, cModel* _model) : cFireworkObject(_particle, _model) {
	implementation->numChildren = 40;
	changeColour(glm::vec3(1.f, 0.f, 0.f));
}

std::vector<cFireworkObject*> cFancyFirework1Object::triggerNextStage() {
	std::vector<cFireworkObject*> newFireworks;
	glm::vec3 direction;
	for (int x = 0; x < implementation->numChildren; x++) {
		direction = worldSpace->getRandomVector3(15.f);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}