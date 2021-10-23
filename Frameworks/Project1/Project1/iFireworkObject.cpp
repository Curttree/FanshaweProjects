#include "iFireworkObject.h"
iFireworkObject::iFireworkObject(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
}
iFireworkObject::iFireworkObject() {
}
iFireworkObject::~iFireworkObject() {
	if (particle) {
 		delete particle;
		particle = NULL;
	}
	if (model) {
		delete model;
		model = NULL;
	}
	if (fuse) {
		delete fuse;
		fuse = NULL;
	}
}
void iFireworkObject::changeColour(glm::vec3 colour) {
	model->vertexColourOverrideHACK = colour;
}