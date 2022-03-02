#pragma once
#include "iParticleContactGenerator.h"

class cParticleParticleContactGenerator : public iParticleContactGenerator {
public:
	cParticleParticleContactGenerator();
	virtual ~cParticleParticleContactGenerator();
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
};