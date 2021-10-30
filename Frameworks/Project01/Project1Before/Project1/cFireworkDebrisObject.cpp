#include "cFireworkDebrisObject.h"
#include "cFirework_Implementation.h"

cFireworkDebrisObject::cFireworkDebrisObject(cFirework* _particle, cModel* _model) : cFireworkObject(_particle, _model) {

}
cFireworkDebrisObject::cFireworkDebrisObject(cFirework* _particle, cModel* _model, glm::vec3 _colour) : cFireworkObject(_particle, _model) {
	changeColour(_colour);
}

std::vector<cFireworkObject*> cFireworkDebrisObject::triggerNextStage() {
	return std::vector<cFireworkObject*>();
}

bool cFireworkDebrisObject::RecieveMessage(sMessage theMessage) {

	return true;
}

bool cFireworkDebrisObject::RecieveMessage(sMessage theMessage, sMessage& theResponse) {

	return true;
}

bool cFireworkDebrisObject::SetReciever(iMediator* pTheReciever) {
	this->p_Mediator = pTheReciever;
	return true;
}