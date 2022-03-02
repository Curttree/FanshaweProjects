#include "cHerbivore.h"
#include "globals.h"
#include "globalFunctions.h"

void cHerbivore::TimeStep(float deltaTime) {
	if (isHungry) {
		LocateFood();
		if (!food) {
			//Couldn't find a food source. Wait until a fruit is ready.
			return;
		}
		position += glm::normalize(food->position - position) * movementSpeed;
		mesh->positionXYZ = position;
		if (glm::distance(position, food->position) < 0.1f) {
			EatFood();
		}
	}
	else {
		cAnimal::TimeStep(deltaTime);
		if (isHungry) {
			mesh->textureNames[0] = "hungry.bmp";
		}
		//Check to see if we have touched the path.
		unsigned int x = static_cast<unsigned int>(position.x);
		unsigned int y = static_cast<unsigned int>(position.z);
		if (!::g_pmazeMaker->maze[x][y][0]) {
			position -= glm::normalize(wanderTarget - position) * movementSpeed;
			mesh->positionXYZ = position;
			SelectNewWanderTarget();
		}
	}

}
void cHerbivore::Born(glm::vec3 location) {
	cAnimal::Born(location);
	mesh->textureNames[0] = "herb.bmp";
	hungryAtTime = ::gGetRandBetween(5.f, 10.f);
	movementSpeed = 0.1f;
}

void cHerbivore::LocateFood() {
	food = 0;
	for (unsigned int index = 0; index < ::g_pEcoSystemManager->plants.get_size(); index++) {
		if (::g_pEcoSystemManager->plants[index]->hasBloomed) {
			if (food == 0) {
				food = ::g_pEcoSystemManager->plants[index];
			}
			else if (glm::distance(position, ::g_pEcoSystemManager->plants[index]->position) < glm::distance(position,food->position)) {
				food = ::g_pEcoSystemManager->plants[index];
			}
		}
	}
}

void cHerbivore::EatFood() {
	food->HasBeenEaten();
	food = 0;
	isHungry = false;
	hungryAtTime = ::gGetRandBetween(10.f, 30.f);
	mesh->textureNames[0] = "herb.bmp";
	SelectNewWanderTarget();
}