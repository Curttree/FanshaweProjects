#include "cBase.h"
#include "globals.h"

cBase::cBase(glm::vec3 _position) : position(_position) {
	mesh = new cMesh;
	mesh->meshName = "animal.ply";
	mesh->textureNames[0] = "blue.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Base";
	mesh->positionXYZ = position;
	mesh->scale = 5.0f;

	g_vec_pMeshes.push_back(mesh);
}

cBase::~cBase() {

}