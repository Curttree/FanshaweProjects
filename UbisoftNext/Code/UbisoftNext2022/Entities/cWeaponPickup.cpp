#include "stdafx.h"
#include <windows.h> 
#include "cWeaponPickup.h"
#include "..\cWorldSpace.h"
#include "..\Projectiles\cRapidWeapon.h"
#include "..\App\app.h"

cWeaponPickup::cWeaponPickup(float posX, float posY, float angle, float scale) {
	this->sprite = App::CreateSprite(WEAPON_SPRITE, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);
	this->SetAngle(angle);
	realPos.x = posX;
	realPos.y = posY;
	realScale = scale;
}
void cWeaponPickup::Shot() {
	//Weapon pickups are not destroyed upon being shot.
	return;
}

void cWeaponPickup::Crash() {
	externalDestruction = true;
	cWorldSpace::Instance()->gameState->IncrementScore(500);
	if (!App::IsSoundPlaying(PICKUP_SOUND)) {
		App::PlaySound(PICKUP_SOUND, false);
	}
	cWorldSpace::Instance()->player->SwapWeapon(new cRapidWeapon(cWorldSpace::Instance()->player));
}

float cWeaponPickup::GetRadius() {
	return radius;
}
