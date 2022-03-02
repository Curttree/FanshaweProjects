#pragma once
#include "cAnimal.h"
#include "cPlant.h"

class cHerbivore : public cAnimal {
public:
	cHerbivore() {};
	virtual void TimeStep(float deltaTime); 
	virtual void Born(glm::vec3 position);
	virtual void LocateFood();
	virtual void EatFood();
protected:
	cPlant* food;
	float poopTimer = 0.f;
	float poopMaxTime = 60.f;
};