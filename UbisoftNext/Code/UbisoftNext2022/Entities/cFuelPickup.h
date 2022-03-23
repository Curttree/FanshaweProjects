#pragma once
#include "cStructure.h"

class cFuelPickup : public cStructure {
public:
	cFuelPickup(float posX, float posY, float angle, float scale = 1.f);
	virtual void Shot();
	virtual void Crash();
	virtual float GetRadius();
protected:
	float radius = 40.f;
};