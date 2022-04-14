#include "cResource.h"
#include "globals.h"

cResource::cResource(glm::vec3 _position) : position(_position) {
	mesh = new cMesh;
	mesh->meshName = "animal.ply";
	mesh->textureNames[0] = "red.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Base";
	mesh->positionXYZ = position;
	mesh->scale = 4.0f;

	g_vec_pMeshes.push_back(mesh);
}

cResource::~cResource() {

}

glm::vec3 cResource::GetPosition() {
	return position;
}

void cResource::Update(float deltaTime) {
	if (collectedBy != NULL) {
		mesh->positionXYZ = collectedBy->GetPosition() + glm::vec3(0.f, 1.f, 0.f);
	}
}

void cResource::Collect(cGatherer* _gatherer) {
	collectedBy = _gatherer;
}

void cResource::Deliver() {
	collectedBy = NULL;
	mesh->scale = 0.f;
	isActive = false;
}