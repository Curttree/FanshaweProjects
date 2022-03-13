#include "stdafx.h"
#include "..\App\App.h"
#include "cEnemyBullet.h"
#include "../cWorldSpace.h"

cEnemyBullet::cEnemyBullet() {

}

cEnemyBullet::cEnemyBullet(Vec2 _position, Vec2 _velocity) {
	killTime = 1250.f;
	this->sprite = App::CreateSprite(PLAYER_BULLET_SPRITE, 1, 1);
	this->velocity = _velocity;
	this->position = _position;

	Vec2 camera = cWorldSpace::Instance()->GetDrawPosition();
	this->sprite->SetPosition(position.x + camera.x, position.y + camera.y);
}

void cEnemyBullet::Update(float deltaTime) {
	cProjectile::Update(deltaTime);

	//Update the position
	position.x += velocity.x;
	position.y += velocity.y;

	//Update the position we are drawing the projectile at.
	Vec2 camera = cWorldSpace::Instance()->GetDrawPosition();
	this->sprite->SetPosition(position.x + camera.x, position.y + camera.y);
}

float cEnemyBullet::GetRadius() {
	return 10.f;
}