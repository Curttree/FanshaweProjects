#include "stdafx.h"
#include "..\Entities\cPlayer.h"
#include "cBasicWeapon.h"
#include "cProjectileFactory.h"
#include "..\cWorldSpace.h"

cBasicWeapon::cBasicWeapon(cPlayer* _owner) {
	owner = _owner;
}

void cBasicWeapon::Update(float deltaTime) {
	if (coolDownTimer < coolDownTime) {
		coolDownTimer += deltaTime;
	}
}

void cBasicWeapon::Fire() {
	if (coolDownTimer < coolDownTime) {
		return;
	}

	coolDownTimer = 0.f;

	cProjectileFactory* projectileFactory = cProjectileFactory::Instance();
	Vec2 velocity;
	Vec2 heading = owner->GetHeading();
	velocity.x = heading.x * 10.f;
	velocity.y = heading.y * 10.f;
	cProjectile* newProjectile = projectileFactory->CreateProjectile(0, owner->GetPosition(), velocity);
	cWorldSpace::Instance()->projectiles.push_back(newProjectile);
}