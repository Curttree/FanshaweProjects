#include "stdafx.h"
#include "..\App\App.h"
#include "cPlayerBullet.h"
#include "../cWorldSpace.h"

cPlayerBullet::cPlayerBullet() {

}

cPlayerBullet::cPlayerBullet(Vec2 _position, Vec2 _velocity) {
	killTime = 3000.f;	
	this->sprite = App::CreateSprite(PLAYER_BULLET_SPRITE, 1, 1);
	this->velocity = _velocity;
	this->position = _position;

	Vec2 camera = cWorldSpace::Instance()->GetDrawPosition();
	this->sprite->SetPosition(position.x + camera.x, position.y + camera.y);
}

void cPlayerBullet::Update(float deltaTime) {
	cProjectile::Update(deltaTime); 

	//Update the position
	position.x += velocity.x;
	position.y += velocity.y;

	//Update the position we are drawing the projectile at.
	Vec2 camera = cWorldSpace::Instance()->GetDrawPosition();
	this->sprite->SetPosition(position.x + camera.x, position.y + camera.y);
}

cGameEntity* cPlayerBullet::GetOwner() {
	return cWorldSpace::Instance()->player;
}

float cPlayerBullet::GetRadius() {
	return 10.f;
}