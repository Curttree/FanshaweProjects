#include "cParticleWorld.h"
#include <iostream>

cParticleWorld::cParticleWorld(size_t _maxContacts, size_t _iterations) : contactResolver(_iterations) {

	shouldCalculateIterations = (_iterations == 0);
	forceRegistry = new cParticleForceRegistry();
	contacts = new cParticleContact[_maxContacts];
	maxContacts = _maxContacts;
}

cParticleWorld::~cParticleWorld() {
	if (forceRegistry) {
		delete forceRegistry;
		forceRegistry = NULL;
	}
	if (contacts) {
		delete contacts;
		contacts = NULL;
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
	if (!particle) {
		return false;
	}
	forceRegistry->DeregisterParticle(particle);

	std::vector<cParticle*>::iterator it = std::find(particles.begin(), particles.end(), particle);
	if (it == particles.end()) {
		return false;
	}
	particles.erase(it);
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

bool cParticleWorld::AddConstraintGenerator(iParticleContactGenerator* generator)
{
	// Check if null or already in list.
	if (!generator || std::find(contactGenerators.begin(), contactGenerators.end(), generator) != contactGenerators.end())
	{
		return false;
	}

	contactGenerators.push_back(generator);
	return true;
}
bool cParticleWorld::RemoveContactContactGenerator(iParticleContactGenerator* generator)
{
	if (!generator)
	{
		return false;
	}
	// Check if generator exists, and if so, get its location
	std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(contactGenerators.begin(), contactGenerators.end(), generator);
	if (itGenerator == contactGenerators.end())
	{
		return false; // nothing was removed.
	}
	contactGenerators.erase(itGenerator);
	return true; // found it. removed it.
}