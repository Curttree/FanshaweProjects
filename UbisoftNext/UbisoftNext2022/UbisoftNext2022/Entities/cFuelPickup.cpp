#include "stdafx.h"
#include <windows.h> 
#include "cFuelPickup.h"
#include "..\cWorldSpace.h"
#include "..\App\app.h"

cFuelPickup::cFuelPickup(float posX, float posY, float angle, float scale) {
	this->sprite = App::CreateSprite(FUEL_SPRITE, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);
	this->SetAngle(angle);
	realPos.x = posX;
	realPos.y = posY;
	realScale = scale;
}
void cFuelPickup::Shot() {
	//Fuel pickups are not destroyed upon being shot.
	return;
}

void cFuelPickup::Crash() {
	externalDestruction = true;
	cWorldSpace::Instance()->gameState->IncrementFuel(500.f);
	cWorldSpace::Instance()->gameState->IncrementScore(250);
	if (!App::IsSoundPlaying(PICKUP_SOUND)) {
		App::PlaySound(PICKUP_SOUND, false);
	}
}

float cFuelPickup::GetRadius() {
	return radius;
}
