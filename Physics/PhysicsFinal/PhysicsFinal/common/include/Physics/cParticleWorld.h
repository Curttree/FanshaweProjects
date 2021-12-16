#pragma once
#include <vector>

#include "cParticle.h"
#include "cParticleForceRegistry.h"
#include "cParticleContact.h"
#include "iParticleContactGenerator.h"
#include "cParticleContactResolver.h"

class cParticleWorld {
public:
	cParticleWorld(size_t _maxContacts, size_t _iterations = 0);
	virtual ~cParticleWorld();

	bool AddParticle(cParticle* particle);
	bool RemoveParticle(cParticle* particle);

	void Update(float deltaTime);

	void IntegrateParticles(float deltaTime); 

	size_t GenerateContacts();

	cParticleForceRegistry* GetForceRegistry() const;

	bool AddConstraintGenerator(iParticleContactGenerator* generator);
	bool RemoveContactContactGenerator(iParticleContactGenerator* generator);

private:
	std::vector<cParticle*> particles;
	cParticleForceRegistry* forceRegistry;
	cParticleContact* contacts;
	size_t maxContacts;
	std::vector<iParticleContactGenerator*> contactGenerators;
	cParticleContactResolver contactResolver;
	// The world should calculate the number of iterations
	// to give the contact resolver each frame.
	bool shouldCalculateIterations = false;
};