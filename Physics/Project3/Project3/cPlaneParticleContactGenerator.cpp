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
		//TODO: Factor in variable radius
		if (glm::abs(particleDistance) < 1.f) {
			contact->contactNormal = normal;
			contact->particle0 = particles[x];
			contact->particle1 = 0;
			contact->penetrationDepth = -particleDistance;
			contact->restitutionCoefficient = 0.7f;	// Come up with own value that seems realistic. Maybe set on particle level.
			contact++;
			createdContacts++;
		}
		if (createdContacts >= limit) {
			return createdContacts;
		}
	}
	return createdContacts;
}