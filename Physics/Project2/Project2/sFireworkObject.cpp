#include "sFireworkObject.h"

sFireworkObject::~sFireworkObject() {
	if (particle) {
		delete particle;
		particle = NULL;
	}
	if (model) {
		delete model;
		model = NULL;
	}
}