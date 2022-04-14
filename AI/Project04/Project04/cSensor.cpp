#include "cSensor.h"
#include "globals.h"


cSensor::cSensor(glm::vec3 _offset, cMesh* _parent) : offset(_offset), parent(_parent) {
	mesh = new cMesh;
	mesh->meshName = "Sphere_xyz_n_rgba_uv.ply";
	mesh->textureNames[0] = "red.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Sensor";
	mesh->positionXYZ = parent->positionXYZ + offset;
	mesh->scale = 0.05f;

	g_vec_pMeshes.push_back(mesh);
}

glm::vec3 cSensor::GetPosition() {
	return mesh->positionXYZ;
}

void cSensor::ResetPosition() {
	mesh->positionXYZ = parent->positionXYZ + offset;
}

void cSensor::UpdateOffset(glm::vec3 newOffset) {
	offset = newOffset;
	ResetPosition();
}
bool cSensor::CheckForCollisions() {
	cMesh* other;
	float penetration;
	bool result = ::g_pConductor->CheckForCollisions(mesh, parent, other, penetration);
	if (result) {
		//mesh->positionXYZ = parent->positionXYZ + (offset * max(1.f - penetration,0.f));
	}
	else {
		mesh->positionXYZ = parent->positionXYZ + offset;
	}
	return result;
}