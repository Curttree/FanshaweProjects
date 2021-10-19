#pragma once

#include <iostream>

#include "cParticle.h"
#include "cParticleWorld.h"
#include "cGravityGenerator.h"

#include "cWorldSpace.h"

class cFirework : public cParticle{
public:
	using cParticle::cParticle;
	virtual bool isInTheAir();
	virtual void update();
	bool hitApex = false;
private:
	static cWorldSpace* _world;
};