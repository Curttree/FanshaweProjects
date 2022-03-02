#include "cEcosystemManager.h"
#include "globals.h"
#include "globalFunctions.h"

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

void cEcosystemManager::TimeStep(float deltaTime) {
	////Do the time step for the plants.
	for (unsigned int index = 0; index < plants.get_size(); index++) {
		plants[index]->TimeStep(deltaTime);
	}
}