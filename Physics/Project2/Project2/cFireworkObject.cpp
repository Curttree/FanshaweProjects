#include "cFireworkDebrisObject.h"
cFireworkObject::cFireworkObject(cFirework* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
}
cFireworkObject::cFireworkObject() {

}
cFireworkObject::~cFireworkObject() {

	//TODO: Fix memory management, there seems to be some overlapping memory references.
	if (particle) {
 		delete particle;
		particle = NULL;
	}
	if (model) {
		delete model;
		model = NULL;
	}
}