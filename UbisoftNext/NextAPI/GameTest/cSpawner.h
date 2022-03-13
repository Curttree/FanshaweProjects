#pragma once
#include "Entities/cPlanet.h"

class cSpawner {
public:
	cSpawner(cPlanet* _planet);
	~cSpawner();
	virtual void Update(float deltaTime);
	virtual void Init();

private:
	cPlanet* planet;
	void SpawnCollectible();
	void SpawnEnemy();

	float collectibleTimer = 20000.f;
	float collectibleTime = 0.f;

	float enemyTimer = 5000.f;
	float enemyTime = 0.f;
};