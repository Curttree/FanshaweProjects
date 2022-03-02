#include "cAnimal.h"
#include "globals.h"
#include "globalFunctions.h"

void cAnimal::TimeStep(float deltaTime) {
	if (!isHungry) {
		hungerTimer+=deltaTime;
		if (hungerTimer >= hungryAtTime) {
			std::cout << "Hungry" << std::endl;
			isHungry = true;
		}
		else {
			position += glm::normalize(wanderTarget - position) * movementSpeed;
			mesh->positionXYZ = position;
			float distance = glm::distance(wanderTarget,position);
			if (distance < 0.25f) {
				std::cout << "Arrived" << std::endl;
				SelectNewWanderTarget();
			}
		}
	}
}
void cAnimal::Born(glm::vec3 location) {
	mesh = new cMesh;
	mesh->meshName = "animal.ply";
	mesh->textureNames[0] = "path.bmp";
	mesh->textureRatios[0] = 1.f;
	mesh->friendlyName = "Herbivore";
	mesh->positionXYZ = location;
	mesh->scale = 1.0f;

	g_vec_pMeshes.push_back(mesh);
	hungerTimer = 0.f;
	position = location;
	SelectNewWanderTarget();
}

void cAnimal::SelectNewWanderTarget() {
	unsigned int x = ::gGetRandBetween(0, 50);
	unsigned int y = ::gGetRandBetween(0, 50);
	wanderTarget = glm::vec3(x * 1.f, position.y, y * 1.f);
}

void cAnimal::LocateFood() {
	//We don't know what to eat.
	return;
}

void cAnimal::EatFood() {
	//We don't know what to eat.
	return;
}