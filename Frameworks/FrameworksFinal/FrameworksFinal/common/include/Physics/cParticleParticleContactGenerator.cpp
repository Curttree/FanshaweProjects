#include "cParticleParticleContactGenerator.h"
#include <iostream>
#include "../../../FrameworksFinal/iMessage.h"		//Poor folder structure here. Make iMessage common in the future.
#include"../../../FrameworksFinal/globals.h"


cParticleParticleContactGenerator::cParticleParticleContactGenerator() {
}

cParticleParticleContactGenerator::~cParticleParticleContactGenerator() {

}

size_t cParticleParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const {
	if (particles.size() == 0) {
		return 0;
	}
	size_t createdContacts = 0;
	float combinedRadius = 0.f;
	float particleDistance = 0.f;
	glm::vec3 contactVector(0.f);

	// Loop through unique pairs of particles until we have exhausted all possibilities.
	for (int x = 0; x < particles.size() - 1; x++) {
		for (int y = x + 1; y < particles.size(); y++) {
			combinedRadius = particles[x]->GetRadius() + particles[y]->GetRadius();
			contactVector = particles[x]->GetPosition() - particles[y]->GetPosition();
			// Check if the positions of each particle are less than the combined radius of the two particles.
			if (glm::length(contactVector) <= combinedRadius && (particles[x]->type != particles[y]->type)) {
				// We don't want to simulate actual contacts. Instead do something.
				sMessage collisionMessage;
				collisionMessage.command = "Hit";
				collisionMessage.vec_voidPData.push_back(particles[x]->owner);
				collisionMessage.vec_voidPData.push_back(particles[y]->owner);

				::g_pInvaderMediator->RecieveMessage(collisionMessage);

				// This contact will simulate particle x colliding with particle y
			
				//contact->contactNormal = glm::normalize(contactVector);
				//contact->particle0 = particles[x];
				//contact->particle1 = particles[y];
				//contact->penetrationDepth = combinedRadius - particleDistance;
				//contact->restitutionCoefficient = 0.85f;	// Slightly higher than particle/plane to make balls bounce off each other more and take longer to pile up in corners.
				//contact++;
				//createdContacts++;
			}
			if (createdContacts >= limit) {
				return createdContacts;
			}
		}
	}
	return createdContacts;
}