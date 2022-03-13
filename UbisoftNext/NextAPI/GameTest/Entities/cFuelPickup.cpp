#include "stdafx.h"
#include "cFuelPickup.h"
#include "..\App\app.h"
#include "..\cWorldSpace.h"

cFuelPickup::cFuelPickup(float posX, float posY, float angle, float scale) {
	this->sprite = App::CreateSprite(SHIP_SPRITES, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);
	this->SetAngle(angle);
	realPos.x = posX;
	realPos.y = posY;
	realScale = scale;
}

void cFuelPickup::Update(float deltaTime) {
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