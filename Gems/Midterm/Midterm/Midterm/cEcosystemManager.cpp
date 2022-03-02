#include "cEcosystemManager.h"
#include "globals.h"
#include "globalFunctions.h"
#include "cHerbivore.h"
#include "cCarnivore.h"

cEcosystemManager::cEcosystemManager() {
}

cEcosystemManager::~cEcosystemManager() {

}

void cEcosystemManager::GeneratePlants(unsigned int count) {
	for (unsigned int counter = 0; counter < count; counter++) {
		bool canPlant = false;
		while (!canPlant) {
			int x_coord = ::gGetRandBetween(0, 50);
			int y_coord = ::gGetRandBetween(0, 50);
			if (::g_pmazeMaker->maze[x_coord][y_coord][0]) {
				//We found grass, we can plant.
				canPlant = true;
				float baseTime = 5.f;
				float randAddition = ::gGetRandBetween(0.f, 4.f);
				float x_add = ::gGetRandBetween(0.2f, 0.8f);
				float y_add = ::gGetRandBetween(0.2f, 0.8f);
				plants.push_back(new cPlant(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add), baseTime + randAddition));
			}
		}
	}
}
void cEcosystemManager::GenerateAnimals(unsigned int herbs, unsigned int carns) {
	for (unsigned int counter = 0; counter < herbs; counter++) {
		bool valid = false;
		while (!valid) {
			int x_coord = ::gGetRandBetween(0, 50);
			int y_coord = ::gGetRandBetween(0, 50);
			if (::g_pmazeMaker->maze[x_coord][y_coord][0]) {
				//We found grass, we can place the animal.
				valid = true;
				float x_add = ::gGetRandBetween(0.2f, 0.8f);
				float y_add = ::gGetRandBetween(0.2f, 0.8f);
				cHerbivore* herbivore = new cHerbivore();
				herbivore->Born(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add));
				herbivores.push_back(herbivore);
			}
		}
	}

	for (unsigned int counter = 0; counter < carns; counter++) {
			int x_coord = ::gGetRandBetween(0, 50);
			int y_coord = ::gGetRandBetween(0, 50);
			float x_add = ::gGetRandBetween(0.2f, 0.8f);
			float y_add = ::gGetRandBetween(0.2f, 0.8f);
			cCarnivore* carnivore = new cCarnivore();
			carnivore->Born(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add));
			carnivores.push_back(carnivore);
	}
}

void cEcosystemManager::TimeStep(float deltaTime) {
	////Do the time step for the plants.
	for (unsigned int index = 0; index < plants.get_size(); index++) {
		plants[index]->TimeStep(deltaTime);
	}
	//TODO: Switch to multithreaded approach.
	////Do the time step for the animals.
	for (unsigned int index = 0; index < herbivores.get_size(); index++) {
		herbivores[index]->TimeStep(deltaTime);
	}
	for (unsigned int index = 0; index < carnivores.get_size(); index++) {
		carnivores[index]->TimeStep(deltaTime);
	}
}