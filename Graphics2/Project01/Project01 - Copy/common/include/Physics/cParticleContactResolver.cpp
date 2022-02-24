#include "cParticleContactResolver.h"

cParticleContactResolver::cParticleContactResolver(size_t _iterations) {
	iterations = _iterations;
}

void cParticleContactResolver::SetIterations(size_t _iterations) {
	iterations = _iterations;
}

void cParticleContactResolver::ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime) {
	
	iterationsUsed = 0;
	while (iterationsUsed < iterations) {

		float max = std::numeric_limits<float>::max();
		size_t indexOfMax = numContacts;
		for (size_t i = 0; i < numContacts; i++) {
			float separationVelocity = contactArray[i].CalculateSeparatingVelocity();
			if (separationVelocity < max && (separationVelocity < 0 || contactArray[i].penetrationDepth > 0)) {
				max = separationVelocity;
				indexOfMax = i;
			}
		}

		if (indexOfMax == numContacts) {
			// No suitable candidates were found to resolve.
			break;
		}

		contactArray[indexOfMax].Resolve(deltaTime);

		cParticleContact* contact = &contactArray[indexOfMax];

		for (size_t i = 0; i < numContacts; i++) {
			if (contactArray[i].particle0 == contact->particle0) {
				contactArray[i].penetrationDepth -= glm::dot(contact->movement0, contactArray[i].contactNormal);
			}
			else if (contactArray[i].particle0 == contact->particle1) {
				contactArray[i].penetrationDepth -= glm::dot(contact->movement1, contactArray[i].contactNormal);
			}
			// Also update the second particle involved in the collision if one exists.
			if (contactArray[i].particle1) {
				if (contactArray[i].particle1 == contact->particle0) {
					contactArray[i].penetrationDepth += glm::dot(contact->movement0, contactArray[i].contactNormal);
				}
				else if (contactArray[i].particle0 == contact->particle1) {
					contactArray[i].penetrationDepth += glm::dot(contact->movement1, contactArray[i].contactNormal);
				}
			}
		}

		// Completed an iteration. Increment value.
		iterationsUsed++;
	}
}