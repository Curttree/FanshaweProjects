#include "stdafx.h"
#include "..\App\app.h"
#include "cSplitBunker.h"
#include "..\Projectiles\cProjectileFactory.h"
#include "../cWorldSpace.h"

cSplitBunker::cSplitBunker(float posX, float posY, float angle, float scale) {
	this->sprite = App::CreateSprite(BUNKER2_SPRITE, 1, 1);
	this->SetPosition(posX, posY);
	this->SetScale(scale);
	this->SetAngle(angle);
	realPos.x = posX;
	realPos.y = posY;
	realScale = scale;
}
void cSplitBunker::Fire() {
	{
		Vec2 direction;
		direction.x = (0.f * cos(angle+0.25f) - 1.f * sin(angle + 0.25f)) * 5.f;
		direction.y = (0.f * sin(angle + 0.25f) + 1.f * cos(angle + 0.25f)) * 5.f;

		Vec2 spawn;
		spawn.x = position.x + (0.f * cos(angle + 0.25f) - 1.f * sin(angle + 0.25f)) * 20.f;
		spawn.y = position.y + (0.f * sin(angle + 0.25f) + 1.f * cos(angle + 0.25f)) * 20.f;

		cProjectile* newProjectile = cProjectileFactory::Instance()->CreateProjectile(1, spawn, direction);
		cWorldSpace::Instance()->projectiles.push_back(newProjectile);
	}
	{
		Vec2 direction;
		direction.x = (0.f * cos(angle - 0.25f) - 1.f * sin(angle - 0.25f)) * 5.f;
		direction.y = (0.f * sin(angle - 0.25f) + 1.f * cos(angle - 0.25f)) * 5.f;

		Vec2 spawn;
		spawn.x = position.x + (0.f * cos(angle - 0.25f) - 1.f * sin(angle - 0.25f)) * 20.f;
		spawn.y = position.y + (0.f * sin(angle - 0.25f) + 1.f * cos(angle - 0.25f)) * 20.f;

		cProjectile* newProjectile = cProjectileFactory::Instance()->CreateProjectile(1, spawn, direction);
		cWorldSpace::Instance()->projectiles.push_back(newProjectile);
	}
}