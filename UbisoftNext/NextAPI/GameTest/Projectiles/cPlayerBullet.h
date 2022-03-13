#pragma once
#include "cProjectile.h"
#include "..\sVec2.h"

class cPlayerBullet : public cProjectile {
public:
	cPlayerBullet(Vec2 _position, Vec2 _velocity);
	void Update(float deltaTime);
protected:
	cPlayerBullet();
};