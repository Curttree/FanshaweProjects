#pragma once
#include "cStructure.h"

class cBunker : public cStructure {
public:
	cBunker(float posX, float posY, float angle, float scale = 1.f);
	virtual float GetRadius();
	virtual void Shot();
	virtual void Update(float deltaTime);
	virtual void Fire();
protected:
	cBunker();
	float radius = 25.f;

	float shotTimer = 1000.f;
	float shotTime = 0.f;
};