#include "cFancyFirework3Object.h"
#include "cFirework_Implementation.h"

cFancyFirework3Object::cFancyFirework3Object(cFirework* _particle, cModel* _model) : cFireworkObject(_particle, _model) {
	implementation->numChildren = 2;
	changeColour(glm::vec3(0.94f, 0.9f, .55f));
}

std::vector<cFireworkObject*> cFancyFirework3Object::triggerNextStage() {
	std::vector<cFireworkObject*> newFireworks;
	float multiplier = 1.5f;
	for (int x = 0; x < implementation->numChildren; x++) {
		newFireworks.push_back(fireworkBuilder->buildFirework(4, particle->GetPosition(), glm::vec3(multiplier, 0.5f, float(x)), model->vertexColourOverrideHACK));
		multiplier *= -1.f;
	}
	return newFireworks;
}
bool cFancyFirework3Object::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cFancyFirework3Object::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cFancyFirework3Object::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}