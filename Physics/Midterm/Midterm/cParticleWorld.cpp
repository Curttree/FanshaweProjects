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

	// 3) Generate contacts
	size_t numContactsGenerated = GenerateContacts();

	// 4) Resolve contacts
	if (numContactsGenerated > 0)
	{
		if (shouldCalculateIterations)
		{
			contactResolver.SetIterations(numContactsGenerated * 2);
		}
		contactResolver.ResolveContacts(contacts, numContactsGenerated, deltaTime);
	}
}

size_t cParticleWorld::GenerateContacts() {
	size_t limit = maxContacts;
	cParticleContact* nextContact = contacts;

	for (iParticleContactGenerator* g : contactGenerators)
	{
		size_t used = g->AddContact(nextContact, limit, particles);
		limit -= used;
		nextContact += used;

		if (limit <= 0)
		{
			std::cout << "We've run out of contacts to fill. Collisions are missing." << std::endl;
			break;
		}
	}

	return maxContacts - limit;
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