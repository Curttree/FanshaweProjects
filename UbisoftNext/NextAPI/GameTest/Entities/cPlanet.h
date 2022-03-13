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

private:
	float radius;
	Vec2 position;
	float mass;
	std::vector<Surface*> surfaces;
};