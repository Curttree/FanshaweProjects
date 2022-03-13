#pragma once
#include "Entities/cPlanet.h"

class cSpawner {
public:
	cSpawner(cPlanet* _planet);
	~cSpawner();
	void Update(float deltaTime);
private:
	cPlanet* planet;

	float collectibleTimer = 5000.f;
	float collectibleTime = 0.f;
};