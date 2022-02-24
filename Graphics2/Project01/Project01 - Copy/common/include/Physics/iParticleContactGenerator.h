#pragma once
#include "cParticleContact.h"
#include <vector>

class iParticleContactGenerator
{
public:

	virtual ~iParticleContactGenerator() {}

	// Returns the number of contacts generated.
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const = 0;
};