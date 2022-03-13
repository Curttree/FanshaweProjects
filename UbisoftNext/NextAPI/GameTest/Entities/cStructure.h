#pragma once
#include "cGameEntity.h"

class cStructure : public cGameEntity {
public:
	virtual void Update(float deltaTime);
	virtual float GetRadius();
	virtual void Shot();
	virtual void Crash();
	virtual bool ShouldDestroy();
protected:
	bool externalDestruction = false;
	Vec2 realPos;
	float realScale;
	float radius = 25.f;
};