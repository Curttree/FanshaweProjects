#include "stdafx.h"
#include "cPlanet.h"
#include "..\app\app.h"
#include "..\cWorldSpace.h"

cPlanet::cPlanet(float _x, float _y, float _radius, float _mass) {
	position.x = _x;
	position.y = _y;
	radius = _radius;
	mass = _mass;
	surfaces.push_back(new Surface{- radius, - radius, - radius, radius });
	surfaces.push_back(new Surface{ - radius, radius, radius, radius });
	surfaces.push_back(new Surface{ radius, radius, radius, - radius });
	surfaces.push_back(new Surface{ radius, - radius, - radius, - radius });
}

cPlanet::~cPlanet() {
	for each (Surface* surface in surfaces)
	{
		delete surface;
		surface = 0;
	}
}

void cPlanet::Draw() {
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	Vec2 camera = worldSpace->GetDrawPosition();
	float scale = worldSpace->GetScale();
	Vec2 drawStartPosition;
	Vec2 drawEndPosition;
	for each (Surface * surface in surfaces)
	{
		//Draw scaling is off. Fix this.
		drawStartPosition.x = (position.x + surface->sX * scale + camera.x) ;
		drawStartPosition.y = (position.y + surface->sY * scale + camera.y);
		drawEndPosition.x = (position.x + surface->eX * scale + camera.x);
		drawEndPosition.y = (position.y + surface->eY * scale + camera.y);
		App::DrawLine(drawStartPosition.x, drawStartPosition.y, drawEndPosition.x, drawEndPosition.y, 0.f, 1.f, 0.f);
	}
}

void cPlanet::Update(float deltaTime) {
}

float cPlanet::GetRadius() {
	return radius;
}

Vec2 cPlanet::GetPosition() {
	return position;
}

bool cPlanet::CheckForCrash(Vec2 otherPos, float otherRadius) {
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	Vec2 camera = worldSpace->GetDrawPosition();
	float scale = worldSpace->GetScale();
	Vec2 startPosition;
	Vec2 endPosition;
	for each (Surface * surface in surfaces) {	
		startPosition.x = (position.x + surface->sX * scale);
		startPosition.y = (position.y + surface->sY * scale);
		endPosition.x = (position.x + surface->eX * scale);
		endPosition.y = (position.y + surface->eY * scale);

		//Distance between point and line segment calculation.
		float distance = 0.f;

		Vec2 AB;
		AB.x = endPosition.x - startPosition.x;
		AB.y = endPosition.y - startPosition.y;

		Vec2 BP;
		BP.x = otherPos.x - endPosition.x;
		BP.y = otherPos.y - endPosition.y;

		Vec2 AP;
		AP.x = otherPos.x - startPosition.x;
		AP.y = otherPos.y - startPosition.y;

		//For dot products.
		float AB_BP, AB_AP;

		AB_BP = (AB.x * BP.x + AB.y * BP.y);
		AB_AP = (AB.x * AP.x + AB.y * AP.y);

		if (AB_BP > 0.f) {
			//Dot product is positive, point must lie in direction of vector, with EndPos being the closest point.
			float  x = otherPos.x - endPosition.x;
			float  y = otherPos.y - endPosition.y;
			distance = sqrt(x * x + y * y);
		}
		else if (AB_AP < 0.f) {
			//Dot product is negative, point must lie in opposite direction of vector, with StartPos being the closest point
			float  x = otherPos.x - startPosition.x;
			float  y = otherPos.y - startPosition.y;
			distance = sqrt(x * x + y * y);
		}
		else {
			//Closest point is somewhere within the surface
			float mod = sqrt(AB.x * AB.x + AB.y * AB.y);
			distance = abs(AB.x * AP.y - AB.y * AP.x) / mod;
		}

		if (distance < otherRadius) {
			return true;
		}
	}
	return false;
}