#pragma once

#include <iostream>

#include <physics/cParticle.h>
#include <physics/cParticleWorld.h>
#include <physics/particle_force_generators.h>

#include "cWorldSpace.h"

class cFirework : public nPhysics::cParticle{
public:
	glm::vec3 colorOverride;
	virtual bool isReadyForStageTwo();
	virtual bool isInTheAir();
	virtual bool isGoingUp();
private:
	static cWorldSpace* _world;
	bool _inTheAir;
	bool _goingUp;
};