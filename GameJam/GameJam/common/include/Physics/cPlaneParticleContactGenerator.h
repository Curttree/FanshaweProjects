#pragma once
#include "iParticleContactGenerator.h"

class cPlaneParticleContactGenerator : public iParticleContactGenerator {
public:
	cPlaneParticleContactGenerator(glm::vec3 _normal, float _distance);
	cPlaneParticleContactGenerator() = delete;	// Deleting default constructor, but keeping copy constructors should we want to generate a series of similar planes.
	virtual ~cPlaneParticleContactGenerator();
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
private:
	glm::vec3 normal = glm::vec3(0.f);
	float distance = 0.f; // Distance from the origin.
};