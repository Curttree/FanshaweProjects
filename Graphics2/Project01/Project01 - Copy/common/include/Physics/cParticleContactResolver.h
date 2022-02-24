#pragma once
#include "cParticleContact.h"

class cParticleContactResolver
{
protected:
	// Holds the number of iterations allowed.
	size_t  iterations = 0;
	// Number of iterations used (for performance tracking)
	size_t iterationsUsed = 0;
public:
	cParticleContactResolver(size_t _iterations);
	void SetIterations(size_t _iterations);
	void ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime);
};