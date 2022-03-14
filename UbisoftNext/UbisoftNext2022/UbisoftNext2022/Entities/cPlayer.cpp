#include "stdafx.h"
#include "cPlayer.h"
#include "..\cWorldSpace.h"
#include <math.h>  

#include <windows.h> 
#include "..\app\app.h"

//TODO: Remove reference once we have weapons being built in factory.
#include "..\Projectiles\cBasicWeapon.h"

cPlayer::cPlayer(float posX, float posY, float scale) {
	this->sprite = App::CreateSprite(SHIP_SPRITES, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);

	this->initialHeading = { 0, 1 };
	this->heading = initialHeading;

	//TODO: Move weapon references to factory if we are going to have multiple.
	this->primaryWeapon = new cBasicWeapon(this);
}

cPlayer::~cPlayer() {
	delete this->sprite;

	if (primaryWeapon) {
		delete this->primaryWeapon;
		this->primaryWeapon = 0;
	}
}

void cPlayer::Update(float deltaTime) {
	if (abs(movement.x) > MOVEMENT_THRESHOLD || abs(movement.y) > MOVEMENT_THRESHOLD) {
		this->SetPosition(position.x + movement.x, position.y + movement.y);
	}
	this->Dampen();
	cGameEntity::Update(deltaTime);

	if (this->primaryWeapon) {
		primaryWeapon->Update(deltaTime);
	}
	if (invincibilityTimer > 0.f) {
		invincibilityTimer -= deltaTime;
	}
}

Vec2 cPlayer::GetHeading() {
	return heading;
}

void cPlayer::Thrust(float percentage) {
	//TODO: Verify this works properly with keyboard/mouse.
	//cWorldSpace* worldSpace = cWorldSpace::Instance();
	//float scale = worldSpace->GetScale();
	movement.x += heading.x * thrustSpeed * percentage;
	movement.y += heading.y * thrustSpeed * percentage;
}

void cPlayer::Pull(Vec2 direction, float magnitude) {
	movement.x += direction.x * magnitude;
	movement.y += direction.y * magnitude;
}

void cPlayer::Dampen() {
	if (abs(movement.x) < MOVEMENT_THRESHOLD && abs(movement.y) < MOVEMENT_THRESHOLD) {
		// Ship is barely moving. Do not bother dampening further.
		return;
	}

	//Support multiple interpretations of dampening, just ensure that the number does not grow larger when trying to slow the ship down.
	if (dampenSpeed - 1.f > 0.f) {
		//Dampening was given as a whole number
		movement.x /= dampenSpeed;
		movement.y /= dampenSpeed;
	}
	else if (dampenSpeed - 1.f < 0.f) {
		//Dampening was given as a percentage.
		movement.x *= dampenSpeed;
		movement.y *= dampenSpeed;
	}
	else{
		//Dampening was given as exactly 1..
		//Do not apply dampening.
	}
}

void cPlayer::Fire(float weaponNumber) {
	//Fire the given weapon..
	if (weaponNumber == 0) {
		primaryWeapon->Fire();
	}
}

void cPlayer::SetAngle(float _angle) {
	//Set the angle for the sprite.
	cGameEntity::SetAngle(_angle);

	//Also update the heading.
	float newX = initialHeading.x * cos(angle) - initialHeading.y * sin(angle);
	float newY = initialHeading.x * sin(angle) + initialHeading.y * cos(angle);

	heading = { newX, newY };
}

float cPlayer::GetRadius() {
	return 10.f;
}

void cPlayer::SwapWeapon(iWeapon* newWeapon) {
	if (primaryWeapon) {
		delete primaryWeapon;
	}
	primaryWeapon = newWeapon;
}

bool cPlayer::Respawn() {
	if (invincibilityTimer > 0.f) {
		return false;
	}
	this->SetPosition(0.f, 200.f);
	this->SwapWeapon(new cBasicWeapon(this));
	cWorldSpace::Instance()->gameState->IncrementLives(-1);
	cWorldSpace::Instance()->gameState->ResetFuel();
	invincibilityTimer = 6000.f;
	return true;
}