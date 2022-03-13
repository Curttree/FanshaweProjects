#include "stdafx.h"
#include "cPlayer.h"
#include <math.h>  

#include "..\App\app.h"

cPlayer::cPlayer(float posX, float posY, float scale) {
	this->sprite = App::CreateSprite(SHIP_SPRITES, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);

	this->initialHeading = { 0, 1 };
	this->heading = initialHeading;
}

cPlayer::~cPlayer() {
	delete this->sprite;
}

void cPlayer::Update(float deltaTime) {
	if (abs(movement.x) > MOVEMENT_THRESHOLD || abs(movement.y) > MOVEMENT_THRESHOLD) {
		this->SetPosition(position.x + movement.x, position.y + movement.y);
	}
	this->Dampen();
	cGameEntity::Update(deltaTime);
}

void cPlayer::Thrust(float percentage) {
	//TODO: Verify this works properly with keyboard/mouse.
	movement.x += heading.x * thrustSpeed * percentage;
	movement.y += heading.y * thrustSpeed * percentage;
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

}

void cPlayer::SetAngle(float _angle) {
	//Set the angle for the sprite.
	cGameEntity::SetAngle(_angle);

	//Also update the heading.
	float newX = initialHeading.x * cos(angle) - initialHeading.y * sin(angle);
	float newY = initialHeading.x * sin(angle) + initialHeading.y * cos(angle);

	heading = { newX, newY };
}