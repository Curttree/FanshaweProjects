#pragma once
#include "cAnimal.h"

class cHerbivore : cAnimal {
public:
	cHerbivore() {};
	virtual void TimeStep(float deltaTime); 
	virtual void Born(glm::vec3 position);
};