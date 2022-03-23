#include "stdafx.h"
#include "cStructure.h"
#include "..\cWorldSpace.h"

void cStructure::Update(float deltaTime) {
	cGameEntity::Update(deltaTime);

	//Recalculate position to ensure we stay on the planet's surface.
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	Vec2 planetPosition = worldSpace->planet->GetPosition();
	float gameScale = worldSpace->GetScale();
	Vec2 newPos;
	newPos.x = planetPosition.x + (realPos.x - planetPosition.x) * gameScale;
	newPos.y = planetPosition.y + (realPos.y - planetPosition.y) * gameScale;

	this->SetPosition(newPos.x, newPos.y);
	this->SetScale(gameScale * realScale);
}

float cStructure::GetRadius() {
	return radius;
}

void cStructure::Shot() {
	externalDestruction = true;
}
void cStructure::Crash() {
	externalDestruction = true;
}
bool cStructure::ShouldDestroy() {
	return externalDestruction;
}