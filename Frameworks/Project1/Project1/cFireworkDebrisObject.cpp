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