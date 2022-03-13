#pragma once
#include "..\sVec2.h"
#include "..\Entities\cGameEntity.h"

class iProjectile {
public:
	iProjectile(){};
	virtual ~iProjectile() {};
	virtual void Update(float deltaTime) = 0;
	virtual void Destroy() = 0;
	virtual void Draw() = 0;
	virtual bool ShouldDestroy() = 0;
	virtual cGameEntity* GetOwner() = 0;
	virtual Vec2 GetPosition() = 0;
	virtual float GetRadius() = 0;
};