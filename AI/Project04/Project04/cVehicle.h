#pragma once
#include "GathererStates.h"
#include "cMesh.h"
#include "Graphs/Graph.h"
#include <extern/glm/vec3.hpp>
#include <list>
#include "cSensor.h"

class cVehicle {
public:
	cVehicle(glm::vec3 _position);
	virtual ~cVehicle();
	void Update(float deltaTime);
	glm::vec3 GetPosition();
	glm::vec3 GetFacing();
	glm::vec3 GetFacing(glm::vec3 otherOrientation);
	cMesh* GetMesh();
private:
	void TurnLeft();
	void TurnRight();
	void UpdateSensors();
	std::vector<float> fuzzyInputs;
	cMesh* mesh;
	glm::vec3 velocity;
	glm::vec3 position;
	std::vector<cSensor*> sensors;
}; 
