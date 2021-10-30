#include "cFireworkObject.h"
#include "cFirework_Implementation.h"
cFireworkObject::cFireworkObject(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
	implementation = new cFirework_Implementation();
}
cFireworkObject::cFireworkObject() {
	implementation = new cFirework_Implementation();
}
cFireworkObject::~cFireworkObject() {
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
	if (implementation) {
		delete implementation;
		implementation = NULL;
	}
}
void cFireworkObject::changeColour(glm::vec3 colour) {
	model->vertexColourOverrideHACK = colour;
}