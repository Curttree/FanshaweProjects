#pragma once
#include <vector>
#include <map>

#include "cParticle.h"
#include "iForceGenerator.h"

class cParticleForceRegistry
{
	typedef std::multimap<cParticle*, iForceGenerator*> registryType;
	typedef std::multimap<cParticle*, iForceGenerator*>::iterator registryIterator;
	typedef std::pair<cParticle*, iForceGenerator*> registryPair;
	typedef std::pair<registryIterator, registryIterator> registryRange;

	registryType registry;
public:
	cParticleForceRegistry();
	~cParticleForceRegistry();

	void Register(cParticle* particle, iForceGenerator* generator);
	void Deregister(cParticle* particle, iForceGenerator* generator);
	void DeregisterParticle(cParticle* particle);
	void DeregisterGenerator(iForceGenerator* generator);

	void UpdateForces(float deltaTime);
};