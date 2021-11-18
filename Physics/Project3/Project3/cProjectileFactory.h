#pragma once
#include "cParticleObject.h"
#include "sParticleTemplate.h"

#define BULLET_PROJ 1
#define LASER_PROJ 2
#define CANNON_PROJ 3
#define ENERGY_PROJ 4
#define RANDOM_SIZE_PROJ 5

class cProjectileFactory {
	public:
		static cProjectileFactory* Instance();
		virtual ~cProjectileFactory();
		bool InitProjectile(int projNumber, sParticleTemplate* proj);
		cParticleObject* createParticle(int projNumber, glm::vec3 direction, glm::vec3 position = glm::vec3(0.f));
	private:
		cProjectileFactory();
		static cProjectileFactory* _instance;
		sParticleTemplate* bullet;
		sParticleTemplate* laser;
		sParticleTemplate* cannonBall;
		sParticleTemplate* energyBall;
};