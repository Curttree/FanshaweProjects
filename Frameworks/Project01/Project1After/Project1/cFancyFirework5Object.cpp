#include "cFancyFirework5Object.h"
#include "cFirework_Implementation.h"

cFancyFirework5Object::cFancyFirework5Object(cFirework* _particle, cModel* _model) : cFireworkObject(_particle, _model) {
	implementation->numChildren = 40;
	implementation->magnitude = 30.f;
	changeColour(glm::vec3(1.f, 0.65f, 0.f));
}

std::vector<cFireworkObject*> cFancyFirework5Object::triggerNextStage() {
	std::vector<cFireworkObject*> newFireworks;
	glm::vec3 direction;
	for (int x = 0; x < implementation->numChildren; x++) {
		direction = worldSpace->getRandomVector3(implementation->magnitude);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}

bool cFancyFirework5Object::RecieveMessage(sMessage theMessage) {
	return true;
}

bool cFancyFirework5Object::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	if (theMessage.command == "MANUAL EXPLOSION") {
		theResponse.vec_fireworkData = triggerNextStage();
	}
	return true;
}

bool cFancyFirework5Object::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}