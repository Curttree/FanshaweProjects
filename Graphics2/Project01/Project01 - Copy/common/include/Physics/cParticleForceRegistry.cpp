#include "cParticleForceRegistry.h"

cParticleForceRegistry::cParticleForceRegistry()
{

}
cParticleForceRegistry::~cParticleForceRegistry()
{

}

void cParticleForceRegistry::Register(cParticle* particle, iForceGenerator* generator)
{
	registry.insert(std::make_pair(particle, generator));
}


void cParticleForceRegistry::Deregister(cParticle* particle, iForceGenerator* generator)
{
	registryRange range = registry.equal_range(particle);
	for (registryIterator x = range.first; x != range.second; )
	{
		registryIterator candidate = x++;
		if (candidate->second == generator)
		{
			registry.erase(candidate);
		}
	}
}


void cParticleForceRegistry::DeregisterParticle(cParticle* particle)
{
	if (!particle) {
		return;
	}
	registry.erase(particle);
}

void cParticleForceRegistry::DeregisterGenerator(iForceGenerator* generator)
{
	for (registryIterator x = registry.begin(); x != registry.end(); )
	{
		registryIterator candidate = x++;
		if (candidate->second == generator)	//Check the generator (second value in pair).
		{
			registry.erase(candidate);
		}
	}
}

void cParticleForceRegistry::UpdateForces(float deltaTime)
{
	for (registryIterator x = registry.begin(); x != registry.end(); x++)
	{
		x->second->Update(x->first, deltaTime);
	}
}