#pragma once
#include <vector>

#include "cParticle.h"
#include "cParticleForceRegistry.h"

class cParticleWorld
{
private:
	std::vector<cParticle*> particles;
	cParticleForceRegistry* forceRegistry;
public:
	cParticleWorld();
	virtual ~cParticleWorld();

	bool AddParticle(cParticle* particle);
	bool RemoveParticle(cParticle* particle);

	void Update(float deltaTime);

	void IntegrateParticles(float deltaTime);

	cParticleForceRegistry* GetForceRegistry() const;
};