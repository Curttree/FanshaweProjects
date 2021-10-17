#include "cFireworkDebrisObject.h"

cFireworkDebrisObject::cFireworkDebrisObject() {
}
cFireworkDebrisObject::cFireworkDebrisObject(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
}
cFireworkDebrisObject::cFireworkDebrisObject(cFirework* _particle, cModel* _model, glm::vec3 _colour) {
	particle = _particle;
	model = _model;
	changeColour(_colour);
}

std::vector<iFireworkObject*> cFireworkDebrisObject::triggerStageTwo() {
	return std::vector<iFireworkObject*>();
}