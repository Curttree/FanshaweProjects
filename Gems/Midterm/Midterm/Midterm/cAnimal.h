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
	virtual bool IsAlive();
	virtual void HasBeenEaten();
	glm::vec3 position;
	// This is static so we can call this outside the function
	// It's for init the critical section for rand
	static void InitCriticalSection(void);
	static void DelCriticalSection(void);
protected:
	bool isHungry = false;
	float hungryAtTime = 0.f;
	float hungerTimer = 0.f;
	float movementSpeed = 0.f;
	bool hasEaten = false;
	bool isAlive = true;
	cMesh* mesh;
	glm::vec3 wanderTarget = glm::vec3(0.f);
};