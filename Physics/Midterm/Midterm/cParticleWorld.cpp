#include "cParticleWorld.h"
#include <iostream>

cParticleWorld::cParticleWorld() {
	forceRegistry = new cParticleForceRegistry();
}

cParticleWorld::~cParticleWorld() {
	if (forceRegistry) {
		delete forceRegistry;
		forceRegistry = NULL;
	}
}

bool cParticleWorld::AddParticle(cParticle* particle) {
	if (!particle || std::find(particles.begin(), particles.end(), particle) != particles.end()) {
		return false;
	}

	particles.push_back(particle);
	return true;
}

bool cParticleWorld::RemoveParticle(cParticle* particle) {
	if (!particle || std::find(particles.begin(), particles.end(), particle) != particles.end()) {
		return false;
	}

	forceRegistry->DeregisterParticle(particle);

	particles.erase(std::find(particles.begin(), particles.end(), particle));
	return true;
}

void cParticleWorld::Update(float deltaTime) {
	// 1) Update Force Generators
	forceRegistry->UpdateForces(deltaTime);

	// 2) Integrate the particles
	IntegrateParticles(deltaTime);

}

void cParticleWorld::IntegrateParticles(float deltaTime)
{
	for (size_t x = 0; x < particles.size(); x++) {
		particles[x]->Integrate(deltaTime);
	}
}

cParticleForceRegistry* cParticleWorld::GetForceRegistry() const
{
	return forceRegistry;
}