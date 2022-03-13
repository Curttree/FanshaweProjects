#pragma once
#include "iProjectile.h"
#include "../sVec2.h"
#include "../App/SimpleSprite.h"
class cProjectile : public iProjectile {
public:
	// From iProjectile
	virtual ~cProjectile();
	virtual void Update(float deltaTime);
	virtual void Destroy();
	virtual void Draw();
	virtual bool ShouldDestroy();
protected:

	CSimpleSprite* sprite;
	float killTime = -1.f;
	float killTimer = 0.f;

	Vec2 position;
	Vec2 velocity;
};