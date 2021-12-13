#pragma once
#include "cParticle.h"

class iForceGenerator
{
public:
	virtual ~iForceGenerator() {}
	virtual void Update(cParticle* particle, float deltaTime) = 0;
};