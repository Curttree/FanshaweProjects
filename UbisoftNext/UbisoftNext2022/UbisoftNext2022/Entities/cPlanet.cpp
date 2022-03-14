#include "stdafx.h"
#include "cPlanet.h"
#include "..\app\app.h"
#include "..\cWorldSpace.h"
#include "..\globalFunctions.h"

cPlanet::cPlanet(float _x, float _y, float _radius, float _mass) {
	position.x = _x;
	position.y = _y;
	radius = _radius;
	mass = _mass;

	//Can be dynamically generated. For now, we are hardcoding.
	surfaces.push_back(new Surface{- radius, - radius, - radius, radius, PI/2.f });
	surfaces.push_back(new Surface{ -radius, radius, -radius * 0.7f, radius, 0.f });

	//Valley
	surfaces.push_back(new Surface{ -radius * 0.7f, radius, -radius * 0.3f, radius*0.9f, -PI / 10.f });
	surfaces.push_back(new Surface{ -radius * 0.3f, radius * 0.9f, radius / 7.f, radius, PI / 20.f });

	//Valley
	surfaces.push_back(new Surface{ radius / 7.f, radius, radius / 4.f, radius * 0.8f, -PI * 0.35f });
	surfaces.push_back(new Surface{ radius / 4.f, radius * 0.8f, radius / 2.f, radius * 0.8f, 0.f });
	surfaces.push_back(new Surface{ radius / 2.f, radius * 0.8f, radius * 0.7f, radius, PI * 0.25f });
	
	surfaces.push_back(new Surface{ radius * 0.7f, radius,radius, radius, 0.f });

	surfaces.push_back(new Surface{ radius, radius, radius * 1.2f, radius * 0.6f,  -PI * 0.35f });
	surfaces.push_back(new Surface{ radius * 1.2f, radius * 0.6f,radius, radius * 0.1f, -PI * 0.65f });

	surfaces.push_back(new Surface{ radius, radius * 0.1f, radius, - radius, -PI / 2.f });

	surfaces.push_back(new Surface{ radius, - radius, - radius, - radius, PI});
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
	Vec2 camera = cWorldSpace::Instance()->GetDrawPosition();
	float scale = cWorldSpace::Instance()->GetScale();
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

float cPlanet::CalculateGravity(Vec2 otherPos, float& distance) {
	// Use simplified version of gravity function.
	distance = sqrt(pow((otherPos.x - position.x), 2) + pow((otherPos.y - position.y), 2));
	return gravityConstant * (mass / pow(distance, 2));
}

void cPlanet::GetSurfacePosition(Vec2& location, float& angle) {
	bool found = false;
	unsigned int maxIterations = 1000;
	unsigned int numIterations = 0;
	while (!found && numIterations < maxIterations) {
		found = true;
		int surfaceIndex = gGetRandBetween(0, (int)surfaces.size());
		float point = gGetRandBetween(0.f, 1.f);
		Vec2 direction;
		direction.x = (surfaces[surfaceIndex]->eX - surfaces[surfaceIndex]->sX);
		direction.y = (surfaces[surfaceIndex]->eY - surfaces[surfaceIndex]->sY);

		location.x = position.x + surfaces[surfaceIndex]->sX + direction.x * point;
		location.y = position.y + surfaces[surfaceIndex]->sY + direction.y * point;

		angle = surfaces[surfaceIndex]->angle;

		for each (cStructure * structure in cWorldSpace::Instance()->structures) {
			if (cWorldSpace::Instance()->ComparePositions(location, structure->GetPosition()) < 100.f) {
				// We are placing the new structure too close to an old one. Try again until we reach max iterations.
				// If we bypass max iterations, the planet is likely too saturated to provide enough spacing. Risk overlapping.
				found = false;
				numIterations++;
			}
		}
	}
}

void cPlanet::GenerateSurfaces(Vec2 startPos, Vec2 endPos, float maxOffset, int segments) {
	Vec2 segmentStart, segmentEnd;
	segmentStart = startPos;
	segmentEnd = endPos;
	for (int x = 0; x < segments; x++) {

	}
}