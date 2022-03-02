#include "cEcosystemManager.h"
#include "globals.h"
#include "globalFunctions.h"
#include "cHerbivore.h"
#include "cCarnivore.h"

cEcosystemManager::cEcosystemManager() {
}

cEcosystemManager::~cEcosystemManager() {
	this->killThreads = true;
	Sleep(100);
	for (unsigned int index = 0; index != this->vec_pAnimalThreads.get_size(); index++)
	{
		if (!this->vec_pAnimalThreads[index]->bIsAlive) {

			delete this->vec_pAnimalThreads[index];
		}
		else {
			//Something went wrong.
			std::cout << "Something's wrong, I can feel it." << std::endl;
		}
	}

	cAnimal::DelCriticalSection();
}

void cEcosystemManager::PlantSeed(glm::vec3 position) {
	float baseTime = 5.f;
	float randAddition = ::gGetRandBetween(0.f, 4.f);
	plants.push_back(new cPlant(position, baseTime + randAddition));
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
				float x_add = ::gGetRandBetween(0.2f, 0.8f);
				float y_add = ::gGetRandBetween(0.2f, 0.8f);
				PlantSeed(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add));
			}
		}
	}
}


DWORD WINAPI AnimalTimeStepWorkerThreadFunction(LPVOID lpParameter)
{
	// "unvoid pointer" the paramers I was sent
	sAnimalThreadDesc* pNewAnimalThreadInfo = (sAnimalThreadDesc*)lpParameter;
	while (!*(pNewAnimalThreadInfo->pKillThreads))
	{
		double deltaTime = 0.0002;	// 20 ms

		// HACK: see if animal object exists
		if (pNewAnimalThreadInfo->pAnimal)
		{
			if (!pNewAnimalThreadInfo->pAnimal->IsAlive()) {
				pNewAnimalThreadInfo->bIsAlive = false;
				return 0;
			}
			// Is the thread running? 
			if (*(pNewAnimalThreadInfo->pRunThreads))
			{
				double deltaTime = 4.0 / 1000.0;	// 4 ms or 250 Hz
				Sleep(4);

				pNewAnimalThreadInfo->pAnimal->TimeStep(deltaTime);
			}
			else
			{
				// Do nothing, but be sure to sleep!
				// (Is it OK to wait 100 ms for the Daleks to "wake up"?
				Sleep(100);
			}
		}
	}
	pNewAnimalThreadInfo->bIsAlive = false;
	return 0;
}
void cEcosystemManager::GenerateAnimals(unsigned int herbs, unsigned int carns) {

	cAnimal::InitCriticalSection();
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
				BirthHerbivore(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add));
			}
		}
	}

	for (unsigned int counter = 0; counter < carns; counter++) {
			int x_coord = ::gGetRandBetween(0, 50);
			int y_coord = ::gGetRandBetween(0, 50);
			float x_add = ::gGetRandBetween(0.2f, 0.8f);
			float y_add = ::gGetRandBetween(0.2f, 0.8f);
			BirthCarnivore(glm::vec3(x_coord * 1.f + x_add, -0.3f, y_coord * 1.f + y_add));
	}
}

void cEcosystemManager::TimeStep(float deltaTime) {
	//Do the time step for the plants.
	for (unsigned int index = 0; index < plants.get_size(); index++) {
		plants[index]->TimeStep(deltaTime);
	}

	//Cleanup any dead threads.
	unsigned int threadCount = vec_pAnimalThreads.get_size();
	for (unsigned int index = 0; index < threadCount; index++) {
		if (vec_pAnimalThreads[index] && !vec_pAnimalThreads[index]->bIsAlive) {
			vec_pAnimalThreads.erase(index);
			threadCount--;
			index--;
		}
	}

	//Cleanup any dead animals.
	unsigned int herbCount = herbivores.get_size();
	for (unsigned int index = 0; index < herbCount; index++) {
		if (!herbivores[index]->IsAlive()) {
			herbivores.erase(index);
			herbCount--;
			index--;
		}
	}
}

void cEcosystemManager::BirthHerbivore(glm::vec3 position) {
	cHerbivore* herbivore = new cHerbivore();
	herbivore->Born(position);

	sAnimalThreadDesc* pNewAnimalThread = new sAnimalThreadDesc();

	pNewAnimalThread->pAnimal = herbivore;

	// Point the bool back to "my" bool
	// Turns the animals on and off (but doesn't exit the threads)
	pNewAnimalThread->pRunThreads = &(this->runThreads);
	// This one kills the threads
	pNewAnimalThread->pKillThreads = &(this->killThreads);

	// Each thread will set this value itself
	pNewAnimalThread->bIsAlive = true;

	// Set up the thread params
	void* pThreadParam = (void*)(pNewAnimalThread);

	// Create a new worker thread and pass this animal to a worker thread 
	pNewAnimalThread->hWorkerThread = CreateThread(
		NULL,		// Not going to change the thread attributes
		0,			// I'm good with the default stack size of 1 Mbyte
		AnimalTimeStepWorkerThreadFunction,		// HAS to be in the LPTHREAD_START_ROUTINE format
		pThreadParam,		// Parameters to pass, type is LPVOID
								// "long pointer to void"
		0,		// Start it right away
		&(pNewAnimalThread->workerThreadID));	// LPDWORD for "long pointer to DWORD"
	vec_pAnimalThreads.push_back(pNewAnimalThread);
	herbivores.push_back(herbivore);
}

void cEcosystemManager::BirthCarnivore(glm::vec3 position) {
	cCarnivore* carnivore = new cCarnivore();
	carnivore->Born(position);
	sAnimalThreadDesc* pNewAnimalThread = new sAnimalThreadDesc();

	pNewAnimalThread->pAnimal = carnivore;

	// Point the bool back to "my" bool
	// Turns the Daleks on and off (but doesn't exit the threads)
	pNewAnimalThread->pRunThreads = &(this->runThreads);
	// This one kills the threads
	pNewAnimalThread->pKillThreads = &(this->killThreads);

	// Each thread will set this value itself
	pNewAnimalThread->bIsAlive = true;

	// Set up the thread params
	void* pThreadParam = (void*)(pNewAnimalThread);

	// Create a new worker thread and pass this Animal to a worker thread 
	pNewAnimalThread->hWorkerThread = CreateThread(
		NULL,		// Not going to change the thread attributes
		0,			// I'm good with the default stack size of 1 Mbyte
		AnimalTimeStepWorkerThreadFunction,		// HAS to be in the LPTHREAD_START_ROUTINE format
		pThreadParam,		// Parameters to pass, type is LPVOID
								// "long pointer to void"
		0,		// Start it right away
		&(pNewAnimalThread->workerThreadID));	// LPDWORD for "long pointer to DWORD"
	vec_pAnimalThreads.push_back(pNewAnimalThread);
	carnivores.push_back(carnivore);
}