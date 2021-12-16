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
			contact->penetrationDepth = particles[x]->GetRadius() - particleDistance;
			// Since human players are currently handling their own restitution coefficient, hardcode for puck. Eventually this should be set per particle/surface.
			contact->restitutionCoefficient = 0.45f;	// Approximation of results from: http://www.dissertations.wsu.edu/Thesis/Spring2008/R_Anderson_042308.pdf
			contact++;
			createdContacts++;
		}
		if (createdContacts >= limit) {
			return createdContacts;
		}
	}
	return createdContacts;
}