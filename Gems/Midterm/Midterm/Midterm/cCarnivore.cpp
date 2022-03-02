#include "cCarnivore.h"
#include "globals.h"
#include "globalFunctions.h"

void cCarnivore::TimeStep(float deltaTime) {
	if (isSleeping) {
		if (!isHungry) {
			hungerTimer += deltaTime;
		}
		sleepTimer += deltaTime;
		if (sleepTimer > sleepingMaxTime) {
			mesh->textureNames[0] = "carn.bmp";
			isSleeping = false;
			sleepTimer = 0.f;
		}
	}
	else {
		awakeTimer += deltaTime;
		if (isHungry) {
			LocateFood();
			if (!food) {
				//We have wiped out the herbivore population. Guess we will have to stay hungry.
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
			//Check to see if we should sleep.
			if (awakeTimer >= sleepCooldown) {
				unsigned int x = static_cast<unsigned int>(position.x);
				unsigned int y = static_cast<unsigned int>(position.z);
				if (::g_pmazeMaker->maze[x][y][0]) {
					//On grass. Should we sleep? Not the best way to determine but it should get the job done.
					unsigned int randValue = ::gGetRandBetween(0, 5);
					if (randValue == 1) {
						isSleeping = true;
						awakeTimer = 0.f;
						mesh->textureNames[0] = "sleep.bmp";
						std::cout << "Someone is sleeping" << std::endl;
					}
				}
			}
		}
	}
}
void cCarnivore::Born(glm::vec3 location) {
	cAnimal::Born(location);
	mesh->textureNames[0] = "carn.bmp";
	hungryAtTime = ::gGetRandBetween(5.f, 10.f);
	movementSpeed = 0.015f;
}
void cCarnivore::LocateFood() {
	food = 0;
	for (unsigned int index = 0; index < ::g_pEcoSystemManager->herbivores.get_size(); index++) {
		if (food == 0) {
			food = ::g_pEcoSystemManager->herbivores[index];
		}
		else if (glm::distance(position, ::g_pEcoSystemManager->plants[index]->position) < glm::distance(position, food->position)) {
			food = ::g_pEcoSystemManager->herbivores[index];
		}
	}
}

void cCarnivore::EatFood() {
	std::cout << "YumYumYum" << std::endl;
	food->HasBeenEaten();
	food = 0;
	isHungry = false;
	hungryAtTime = ::gGetRandBetween(30.f, 50.f);
	SelectNewWanderTarget();
}