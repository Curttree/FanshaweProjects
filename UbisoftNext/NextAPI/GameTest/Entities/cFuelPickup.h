#pragma once
#include "cGameEntity.h"

class cFuelPickup : public cGameEntity {
public:
	cFuelPickup(float posX, float posY, float angle, float scale = 1.f);
	virtual void Update(float deltaTime);
private:
	Vec2 realPos;
	float realScale;
};