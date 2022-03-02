#pragma once
#include "iAnimal.h"

class cAnimal : iAnimal {
public:
	cAnimal() {};
	~cAnimal() {};
	virtual void TimeStep(float deltaTime);
	virtual void Born(glm::vec3 position);
private:
	bool isHungry = false;
	float hungryAtTime = 0.f;
	float hungerTimer = 0.f;
};