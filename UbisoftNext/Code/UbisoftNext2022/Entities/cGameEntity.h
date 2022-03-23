#pragma once
#include "../App/SimpleSprite.h"
#include "../sVec2.h"

// Wrapper for game objects. Contains common elements for all entities present in the game.
// Limiting external access to some sprite functionality so the entity has more control over how/where it's sprite can be updated.
class cGameEntity {
public:
	virtual void Draw();
	virtual void Update(float deltaTime);
	virtual float GetAngle();
	virtual Vec2 GetPosition();
	virtual void GetPosition(float& x, float& y);
	virtual float GetScale();
	virtual void SetAngle(float _angle);
	virtual void SetPosition(float _positionX, float _positionY);
	virtual void SetScale(float _scale);
	virtual float GetRadius();
protected:
	CSimpleSprite* sprite;
	float angle;
	Vec2 position;
	float scale;
};