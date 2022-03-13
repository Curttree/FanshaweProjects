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

}

bool cProjectile::ShouldDestroy() {
	//Destroy if we are past our kill time, or in the event of ininfite 
	return killTimer >= killTime;
}