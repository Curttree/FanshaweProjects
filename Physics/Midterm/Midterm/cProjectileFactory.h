#pragma once
#include "sParticleTemplate.h"

#define BULLET_PROJ 0
#define LASER_PROJ 1
#define CANNON_PROJ 2
#define ENERGY_PROJ 3

class cProjectileFactory {
	public:
		static cProjectileFactory* Instance();
		virtual ~cProjectileFactory();
		bool InitProjectile(int projNumber, sParticleTemplate* proj);
	private:
		cProjectileFactory();
		static cProjectileFactory* _instance;
		sParticleTemplate* bullet;
		sParticleTemplate* laser;
		sParticleTemplate* cannonBall;
		sParticleTemplate* energyBall;
};