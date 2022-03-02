#include "cPlant.h"
#include "globals.h"

cPlant::cPlant(glm::vec3 location, float _maturityTime) {
	maturityTime = _maturityTime;
	mesh = new cMesh;
	mesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	mesh->textureNames[0] = "grass.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Plant";
	mesh->positionXYZ = location;
	mesh->scale = 0.0f;

	g_vec_pMeshes.push_back(mesh);
	elapsedTime = 0.f;
	position = location;
}

cPlant::cPlant() {

}
cPlant::~cPlant() {

}
void cPlant::TimeStep(float deltaTime) {
	if (hasBloomed) {
		//We have bloomed and are waiting for an animal to eat the fruit.
		return;
	}
	this->elapsedTime += deltaTime;
	mesh->scale = elapsedTime / maturityTime * mature_size;
	if (elapsedTime >= maturityTime) {
		hasBloomed = true;
		mesh->textureNames[0] = "fruit.bmp";
	}
}

void cPlant::HasBeenEaten() {
	hasBloomed = false;
	elapsedTime = 0.f;
	mesh->textureNames[0] = "grass.bmp";
}