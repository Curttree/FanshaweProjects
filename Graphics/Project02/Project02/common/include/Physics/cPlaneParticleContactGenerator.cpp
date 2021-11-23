#include "cPlaneParticleContactGenerator.h"
#include <iostream>


cPlaneParticleContactGenerator::cPlaneParticleContactGenerator(glm::vec3 _normal, float _distance) {
	normal = _normal;
	distance = _distance;
}

cPlaneParticleContactGenerator::~cPlaneParticleContactGenerator() {

}

size_t cPlaneParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const {
	size_t createdContacts = 0;
	glm::vec3 particlePosition;
	for (int x = 0; x < particles.size(); x++) {
		float particleDistance = glm::dot(particles[x]->GetPosition(), glm::normalize(normal)) - distance;
		if (glm::abs(particleDistance) < particles[x]->GetRadius()) {
			contact->contactNormal = normal;
			contact->particle0 = particles[x];
			contact->particle1 = 0;
			contact->penetrationDepth = -particleDistance;
			contact->restitutionCoefficient = 0.688f;	// Calculated for hard plastic ball. Sourced from: https://hypertextbook.com/facts/2006/restitution.shtml
			contact++;
			createdContacts++;
		}
		if (createdContacts >= limit) {
			return createdContacts;
		}
	}
	return createdContacts;
}