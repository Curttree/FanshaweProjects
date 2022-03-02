#pragma once
#include <Windows.h>	
#include <process.h>	// Actual thread calls
#include "cPlant.h"
#include <Containers/cCurtArray.h>
#include <Containers/cCurtMap.h>
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3
class cEcosystemManager {
public:
	cEcosystemManager();
	~cEcosystemManager();
	void GeneratePlants(unsigned int count);
	void TimeStep(float deltaTime);

private:
	cCurtArray<cPlant*> plants = cCurtArray<cPlant*>();
};