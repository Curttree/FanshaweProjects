#pragma once
#include <vector>

#include "cParticle.h"
#include "cParticleForceRegistry.h"
#include "cParticleContact.h"
#include "iParticleContactGenerator.h"

class cParticleWorld {
public:
	cParticleWorld();
	virtual ~cParticleWorld();

	bool AddParticle(cParticle* particle);
	bool RemoveParticle(cParticle* particle);

	void Update(float deltaTime);

	void IntegrateParticles(float deltaTime); 

	size_t GenerateContacts();

	cParticleForceRegistry* GetForceRegistry() const;

private:
	std::vector<cParticle*> particles;
	cParticleForceRegistry* forceRegistry;
	cParticleContact* contacts;
	size_t maxContacts;
	std::vector<iParticleContactGenerator*> contactGenerators;
	// The world should calculate the number of iterations
	// to give the contact resolver each frame.
	bool shouldCalculateIterations;
};