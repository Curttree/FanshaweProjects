#pragma once

#include <iostream>

#include <physics/cParticle.h>
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>

#include "cWorldSpace.h"

class cFirework : public nPhysics::cParticle{
public:
	using nPhysics::cParticle::cParticle;
	virtual bool isInTheAir();
	bool hitApex = false;
private:
	static cWorldSpace* _world;
};