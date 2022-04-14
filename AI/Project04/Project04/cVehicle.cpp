#include "cVehicle.h"
#include "globals.h"
#include <iostream>

cVehicle::cVehicle(glm::vec3 _position) : position(_position) {
	mesh = new cMesh;
	mesh->meshName = "cone.ply";
	mesh->textureNames[0] = "green.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Vehicle";
	mesh->positionXYZ = position;
	mesh->scale = 0.25f;

	g_vec_pMeshes.push_back(mesh);
	::g_pConductor->AddObject(mesh);
	velocity = GetFacing() * 0.1f;

	//Create sensors.

	//Forward sensor
	cSensor* sensor = new cSensor(GetFacing(), mesh);
	sensors.push_back(sensor);
	fuzzyInputs.push_back(1.f);

	//Left side sensors.
	cSensor* sensorL30 = new cSensor(GetFacing(mesh->orientationXYZ + glm::vec3(0.f, glm::pi<float>()/6.f, 0.f)), mesh);
	sensors.push_back(sensorL30);
	fuzzyInputs.push_back(1.f);

	cSensor* sensorL60 = new cSensor(GetFacing(mesh->orientationXYZ + glm::vec3(0.f, glm::pi<float>() / 3.f, 0.f)), mesh);
	sensors.push_back(sensorL60);
	fuzzyInputs.push_back(1.f);

	//Right side sensors.
	cSensor* sensorR30 = new cSensor(GetFacing(mesh->orientationXYZ - glm::vec3(0.f, glm::pi<float>() / 6.f, 0.f)), mesh);
	sensors.push_back(sensorR30);
	fuzzyInputs.push_back(1.f);

	cSensor* sensorR60 = new cSensor(GetFacing(mesh->orientationXYZ - glm::vec3(0.f, glm::pi<float>() / 3.f, 0.f)), mesh);
	sensors.push_back(sensorR60);
	fuzzyInputs.push_back(1.f);
}

cVehicle::~cVehicle() {
	//if (localGraph) {
	//	delete localGraph;
	//	localGraph = NULL;
	//}
}

glm::vec3 cVehicle::GetPosition() {
	return position;
}

void cVehicle::Update(float deltaTime) {
	float magnitude = 0.1f;
	//sensors[0]->ResetPosition();

	if (sensors[0]->CheckForCollisions()) {
		// Speed fuzzy logic calculation. Distance is capped at 0, so no additional logic is called.
		magnitude *= min(1.f, glm::distance(position, sensors[0]->GetPosition()));
	}

	//Facing logic
	//Update the inputs first.
	for (unsigned int index = 1; index < sensors.size(); index++) {
		sensors[index]->CheckForCollisions();
	}
	for (unsigned int index = 0; index < sensors.size(); index++) {
		fuzzyInputs[index] = min(1.f, glm::distance(position, sensors[0]->GetPosition()));
	}
	
	if (fuzzyInputs[0] == 1.f || fuzzyInputs[0] == fuzzyInputs[1] == fuzzyInputs[2] == fuzzyInputs[3] == fuzzyInputs[4]) {
		// Realistically can only enter this state if there is no collision ahead, or the almost impossible odds that there are are equidistant collisions coming from all ends.
		// Adjust speed if necessary but stay the course.

		//Only reset position if we are actually moving forward. Logic could be simplified here..
		if (fuzzyInputs[0] == 1.f) {
			for (cSensor* sensor : sensors) {
				sensor->ResetPosition();
			}
		}

	}
	else if (fuzzyInputs[0] < 1.f && fuzzyInputs[1] == 1.f && fuzzyInputs[2] == 1.f) {
		//Collision detected ahead, but left side is clear. Rotate to the left.
		TurnLeft();
	}
	else if (fuzzyInputs[0] < 1.f && fuzzyInputs[3] == 1.f && fuzzyInputs[4] == 1.f) {
		//Forward and left both have some obstruction, but right is clear. Rotate right.
		TurnRight();
	}
	// Left, right, and forward all have some potential collision. Find the least bad option
	else if (fuzzyInputs[1] + fuzzyInputs[2] < fuzzyInputs[3] + fuzzyInputs[4]) {
		//Object(s) on the left are further away, turn to the left and hope we can make further adjustments before slowing too much.
		TurnLeft();
	}
	else {
		//Object(s) on the right are further away, turn to the right and hope we can make further adjustments before slowing too much.
		TurnRight();
	}

	velocity = GetFacing() * magnitude;
	position += velocity;
	mesh->positionXYZ = position;
}

glm::vec3 cVehicle::GetFacing() {
	return glm::vec3(sin(mesh->orientationXYZ.y) * cos(mesh->orientationXYZ.x), sin(mesh->orientationXYZ.x), cos(mesh->orientationXYZ.y) * cos(mesh->orientationXYZ.x));
}

glm::vec3 cVehicle::GetFacing(glm::vec3 otherOrientation) {
	return glm::vec3(sin(otherOrientation.y) * cos(otherOrientation.x), sin(otherOrientation.x), cos(otherOrientation.y) * cos(otherOrientation.x));
}

cMesh* cVehicle::GetMesh() {
	return mesh;
}

void cVehicle::TurnLeft() {
	float turnDistance = glm::radians(min(45.f, 45.f * (fuzzyInputs[2] / max(0.00001, fuzzyInputs[1]))));
	mesh->orientationXYZ += glm::vec3(0.f, turnDistance, 0.f);
	UpdateSensors();
}

void cVehicle::TurnRight() {
	float turnDistance = glm::radians(min(45.f, 45.f * (fuzzyInputs[2] / max(0.00001, fuzzyInputs[1]))));
	mesh->orientationXYZ -= glm::vec3(0.f, turnDistance, 0.f);
	UpdateSensors();
}

void cVehicle::UpdateSensors() {

	sensors[0]->UpdateOffset(GetFacing());
	sensors[1]->UpdateOffset(GetFacing(mesh->orientationXYZ + glm::vec3(0.f, glm::pi<float>() / 6.f, 0.f)));
	sensors[2]->UpdateOffset(GetFacing(mesh->orientationXYZ + glm::vec3(0.f, glm::pi<float>() / 3.f, 0.f)));
	sensors[3]->UpdateOffset(GetFacing(mesh->orientationXYZ - glm::vec3(0.f, glm::pi<float>() / 6.f, 0.f)));
	sensors[4]->UpdateOffset(GetFacing(mesh->orientationXYZ - glm::vec3(0.f, glm::pi<float>() / 3.f, 0.f)));
}