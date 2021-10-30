#include "cFancyFirework2Object.h"
#include "cFirework_Implementation.h"

cFancyFirework2Object::cFancyFirework2Object(cFirework* _particle, cModel* _model) : cFireworkObject(_particle, _model) {
	implementation->numChildren = 20;
	changeColour(glm::vec3(0.f, 1.f, 0.f));
}

std::vector<cFireworkObject*> cFancyFirework2Object::triggerNextStage() {
	std::vector<cFireworkObject*> newFireworks;
	glm::vec3 direction;
	for (int x = 0; x < implementation->numChildren; x++) {
		direction = worldSpace->getRandomVector3(5.f, true, glm::pi<float>() / 3.f);
		newFireworks.push_back(fireworkBuilder->buildFirework(0, particle->GetPosition(), direction, model->vertexColourOverrideHACK));
	}
	return newFireworks;
}

bool cFancyFirework2Object::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cFancyFirework2Object::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cFancyFirework2Object::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}