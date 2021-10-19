#include "cProjectileFactory.h"

cProjectileFactory* cProjectileFactory::_instance = 0;
cProjectileFactory::cProjectileFactory() {

}

cProjectileFactory::~cProjectileFactory() {
	if (bullet) {
		delete bullet;
		bullet = 0;
	}
	if (laser) {
		delete laser;
		laser = 0;
	}
	if (cannonBall) {
		delete cannonBall;
		cannonBall = 0;
	}
	if (energyBall) {
		delete energyBall;
		energyBall = 0;
	}
}

// Take the template information provided and use it to define the given projectile. Return true if we were able to define a valid projectile, false otherwise.
bool cProjectileFactory::InitProjectile(int projectileNumber, sParticleTemplate* proj) {
	switch (projectileNumber) {
	case(BULLET_PROJ):
		bullet = proj;
		break;
	case(LASER_PROJ):
		laser = proj;
		break;
	case(CANNON_PROJ):
		cannonBall = proj;
		break;
	case(ENERGY_PROJ):
		energyBall = proj;
		break;
	default:
		return false;
		break;
	}
	return true;
}

cParticleObject* cProjectileFactory::createParticle(int projNumber, glm::vec3 direction, glm::vec3 position) {
	cParticleObject* result = 0;

	//Define shared attributes.
	cModel* model = new cModel();
	model->modelName = "assets/ball.ply";
	model->bOverriveVertexColourHACK = true;
	model->bIsWireframe = false;
	model->bDontLightObject = false;

	switch (projNumber) {
	case(BULLET_PROJ):
		result = new cParticleObject(bullet, model, direction, position);
		break;
	case(LASER_PROJ):
		result = new cParticleObject(laser, model, direction, position);
		break;
	case(CANNON_PROJ):
		result = new cParticleObject(cannonBall, model, direction, position);
		break;
	case(ENERGY_PROJ):
		result = new cParticleObject(energyBall, model, direction, position);
		break;
	default:
		break;
}
return result;

}

cProjectileFactory* cProjectileFactory::Instance() {
	if (_instance == 0) {
		_instance = new cProjectileFactory;
	}

	return _instance;
}