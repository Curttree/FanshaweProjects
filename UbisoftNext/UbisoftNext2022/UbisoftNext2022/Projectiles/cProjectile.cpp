#include "stdafx.h"
#include <limits>
#include "cProjectile.h"

cProjectile::~cProjectile() {
	if (sprite) {
		delete sprite;
		sprite = 0;
	}
}

void cProjectile::Draw() {
	this->sprite->Draw();
}

void cProjectile::Update(float deltaTime) {
	// Only increment kill timer if we are under the upper limit. Unlikely edge case, but we do have projectiles that will never reach their kill timer.
	if (std::numeric_limits<float>::max() - killTimer > deltaTime) {
		killTimer += deltaTime;
	}
}
void cProjectile::Destroy() {
	//Something else told us to destroy this projectile. Flag it so it will be cleaned up next update loop.
	externalDestruction = true;
}

bool cProjectile::ShouldDestroy() {
	//Destroy if we are past our kill time, or in the event of ininfite 
	return killTimer >= killTime || externalDestruction;
}

cGameEntity* cProjectile::GetOwner() {
	// We either don't have an owner or don't know who they are.
	return 0;
}

float cProjectile::GetRadius() {
	// This is declared on a per projectile basis.
	return 0;
}

Vec2 cProjectile::GetPosition() {
	// This is declared on a per projectile basis.
	return position;
}