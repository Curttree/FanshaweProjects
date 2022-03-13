#include "stdafx.h"
#include "..\App\app.h"
#include "cBunker.h"
#include "..\Projectiles\cProjectileFactory.h"
#include "../cWorldSpace.h"

cBunker::cBunker(float posX, float posY, float angle, float scale) {
	this->sprite = App::CreateSprite(BUNKER_SPRITE, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);
	this->SetAngle(angle);
	realPos.x = posX;
	realPos.y = posY;
	realScale = scale;
}

float cBunker::GetRadius() {
	return radius;
}

void cBunker::Shot() {
	cStructure::Shot();
	cWorldSpace::Instance()->gameState->IncrementScore(100);
}

void cBunker::Update(float deltaTime) {
	cStructure::Update(deltaTime);

	if (shotTime >= shotTimer) {
		Fire();
		shotTime = 0.f;
	}
	else {
		shotTime += deltaTime;
	}
}

void cBunker::Fire() {
	Vec2 direction;
	direction.x = (0.f* cos(angle) - 1.f * sin(angle)) * 10.f;
	direction.y = (0.f * sin(angle) + 1.f * cos(angle)) * 10.f;

	Vec2 spawn;
	spawn.x = position.x + (0.f * cos(angle) - 1.f * sin(angle)) * 20.f;
	spawn.y = position.y + (0.f * sin(angle) + 1.f * cos(angle)) * 20.f;

	cProjectile* newProjectile = cProjectileFactory::Instance()->CreateProjectile(1, spawn, direction);
	cWorldSpace::Instance()->projectiles.push_back(newProjectile);
}