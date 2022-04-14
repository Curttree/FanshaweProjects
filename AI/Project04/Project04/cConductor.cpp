#include "cConductor.h"
#include <iostream>

void cConductor::AddObject(cMesh* obj) {
	objects.push_back(obj);
}

bool cConductor::CheckForCollisions(cMesh* object, cMesh* ignore, cMesh*& other, float& penetration) {
	float minDistance = 0.75f;
	float magnitude = 0.f;
	for (cMesh* obj : objects)
	{
		if (obj == object || obj == ignore) {
			continue;
		}
		magnitude = glm::distance(object->positionXYZ, obj->positionXYZ);
		if (magnitude <= minDistance) {
			other = obj;
			penetration = minDistance - magnitude;
			return true;
		}
	}
	//Iterated through all objects. No contacts detected.
	return false;
}