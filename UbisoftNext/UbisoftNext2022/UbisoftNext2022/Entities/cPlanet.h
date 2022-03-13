#pragma once
#include "../sVec2.h"
#include <vector>
#include "..\sSurface.h"

class cPlanet {
public:
	cPlanet(float _x, float _y, float _radius, float _mass = 1.f);
	virtual ~cPlanet();
	void Draw();
	void Update(float deltaTime);

	Vec2 GetPosition();
	float GetRadius();
	//TODO: Handle via mediator.
	bool CheckForCrash(Vec2 otherPos, float otherRadius);
	float CalculateGravity(Vec2 otherPos, float& distance);
	void GetSurfacePosition(Vec2& location, float& angle);
private:
	float radius;
	Vec2 position;
	float mass;
	std::vector<Surface*> surfaces;

	// Through playtesting, we should determine if this is indeed a constant across planets, and should be moved to a more suitable location.
	float gravityConstant = 0.0025f;
};