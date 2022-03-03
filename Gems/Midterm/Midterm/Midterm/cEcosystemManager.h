#pragma once
#include <Windows.h>	
#include <process.h>	// Actual thread calls
#include "cPlant.h"
#include "cHerbivore.h"
#include "cCarnivore.h"
#include <Containers/cCurtArray.h>
#include <Containers/cCurtMap.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
#include "sAnimalThreadDesc.cpp"
class cEcosystemManager {
public:
	cEcosystemManager();
	~cEcosystemManager();
	void GeneratePlants(unsigned int count);
	void GenerateAnimals(unsigned int herbs, unsigned int carns);
	void BirthHerbivore(glm::vec3 position);
	void BirthCarnivore(glm::vec3 position);
	void PlantSeed(glm::vec3 position);
	void TimeStep(float deltaTime);

	cCurtArray<cPlant*> plants = cCurtArray<cPlant*>();
	cCurtArray<cHerbivore*> herbivores = cCurtArray<cHerbivore*>();
	cCurtArray<cCarnivore*> carnivores = cCurtArray<cCarnivore*>();

	cCurtArray<cCurtArray<cCurtArray<bool>>> paths;
	cCurtArray< sAnimalThreadDesc* > vec_pAnimalThreads;
	bool runThreads;
	bool killThreads;
};