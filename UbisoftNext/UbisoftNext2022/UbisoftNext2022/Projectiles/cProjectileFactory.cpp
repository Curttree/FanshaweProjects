#include "stdafx.h"
#include "cProjectileFactory.h"

// Projectile Types
#include "cPlayerBullet.h"
#include "cEnemyBullet.h"

cProjectileFactory* cProjectileFactory::_instance = 0;
cProjectileFactory::cProjectileFactory() {

}
cProjectileFactory* cProjectileFactory::Instance() {
	if (_instance == 0) {
		_instance = new cProjectileFactory;
	}

	return _instance;
}

cProjectile* cProjectileFactory::CreateProjectile(int type, Vec2 position, Vec2 velocity) {
	cProjectile* proj = 0;
	switch (type) {
	case(0):
		proj = new cPlayerBullet(position, velocity);
		break;
	case(1):
		proj = new cEnemyBullet(position, velocity);
		break;
	default:
		// Unknown projectile. Return empty pointer.
		break;
	}
	return proj;
}