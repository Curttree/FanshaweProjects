#pragma once
#include "iAnimal.h"
#include "cMesh.h"

class cAnimal : public iAnimal {
public:
	cAnimal() {};
	~cAnimal() {};
	virtual void TimeStep(float deltaTime);
	virtual void Born(glm::vec3 location);
	virtual void SelectNewWanderTarget();
	virtual void LocateFood();
	virtual void EatFood();
	glm::vec3 position;
protected:
	bool isHungry = false;
	float hungryAtTime = 0.f;
	float hungerTimer = 0.f;
	float movementSpeed = 0.f;
	cMesh* mesh;
	glm::vec3 wanderTarget = glm::vec3(0.f);
};