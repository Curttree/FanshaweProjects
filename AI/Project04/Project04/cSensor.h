#pragma once
#include <extern/glm/vec3.hpp>
#include "cMesh.h"
class cVehicle;	//Forward declaration of vehicle.

class cSensor {
public:
	cSensor(glm::vec3 _position, cMesh* _parent);
	glm::vec3 GetPosition();
	void ResetPosition();
	void UpdateOffset(glm::vec3 newOffset);
	bool CheckForCollisions();
private:
	cMesh* parent;
	cMesh* mesh;
	glm::vec3 offset;
};