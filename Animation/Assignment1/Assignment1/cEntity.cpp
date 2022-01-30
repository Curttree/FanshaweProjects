#include "cEntity.h"

cEntity::cEntity() {

}

cEntity::~cEntity() {

}

void cEntity::TimeStep(float deltaTime) {
	if (!this->mesh) {
		return;
	}

	// When physics are included, we will update transform properties here based on physics simulation.

	// Update the model with the new values. This would be an area to optimize if we see performance hiccups.
	this->mesh->positionXYZ = position;
	this->mesh->scale = scale;
	this->mesh->orientationXYZ = glm::eulerAngles(rotation);
}