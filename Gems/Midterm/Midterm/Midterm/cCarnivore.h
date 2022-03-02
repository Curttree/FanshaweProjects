#pragma once
#include "cAnimal.h"

class cCarnivore : public cAnimal {
public:
	cCarnivore() {};
	virtual void TimeStep(float deltaTime);
	virtual void Born(glm::vec3 position);
	virtual void LocateFood();
	virtual void EatFood();
private:
	bool isSleeping;
	float sleepingMaxTime = 3.f;
	float sleepTimer = 0.f;
	float awakeTimer = 0.f;
	float sleepCooldown = 3.f;
	cAnimal* food;
};