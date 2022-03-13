#pragma once
#include "../App/SimpleSprite.h"
#include "../sVec2.h"

// Wrapper for game objects. Contains common elements for all entities present in the game.
// Limiting external access to some sprite functionality so the entity has more control over how/where it's sprite can be updated.
class cGameEntity {
public:
	void Draw();
	void Update(float deltaTime);
	float GetAngle();
	Vec2 GetPosition();
	void GetPosition(float& x, float& y);
	float GetScale();
	void SetAngle(float _angle);
	void SetPosition(float _positionX, float _positionY);
	void SetScale(float _scale);
protected:
	CSimpleSprite* sprite;
	float angle;
	Vec2 position;
	float scale;
};