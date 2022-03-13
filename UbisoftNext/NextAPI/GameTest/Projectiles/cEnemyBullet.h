#pragma once
#include "cProjectile.h"
#include "..\sVec2.h"

class cEnemyBullet : public cProjectile {
public:
	cEnemyBullet(Vec2 _position, Vec2 _velocity);
	virtual void Update(float deltaTime);
	virtual float GetRadius();
protected:
	cEnemyBullet();
};