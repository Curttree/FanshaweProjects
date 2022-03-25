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