#pragma once
#include "cProjectile.h"
#include "..\sVec2.h"

class cPlayerBullet : public cProjectile {
public:
	cPlayerBullet(Vec2 _position, Vec2 _velocity);
	virtual void Update(float deltaTime);
	virtual cGameEntity* GetOwner();
	virtual float GetRadius();
protected:
	cPlayerBullet();
};