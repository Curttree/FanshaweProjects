#include "cFancyFirework4Object.h"

cFancyFirework4Object::cFancyFirework4Object() {
}
cFancyFirework4Object::cFancyFirework4Object(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	changeColour(glm::vec3(0.94f, 0.9f, .55f));
}

std::vector<cFireworkObject*> cFancyFirework4Object::triggerNextStage() {
	std::vector<cFireworkObject*> newFireworks;
	float multiplier = 1.f;
	glm::vec3 direction;
	for (int x = 0; x < 20; x++) {
		direction = worldSpace->getRandomVector3(3.f);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}

bool cFancyFirework4Object::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cFancyFirework4Object::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cFancyFirework4Object::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}