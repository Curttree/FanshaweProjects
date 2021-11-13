#include "cParticleContact.h"


void cParticleContact::Resolve(float deltaTime)
{
	ResolveVelocity(deltaTime);
	ResolvePosition(deltaTime);
}

float cParticleContact::GetRestitutionCoefficient()
{
	return restitutionCoefficient;
}

void cParticleContact::SetRestitutionCoefficient(float newRestitution)
{
	// Restitution coefficient should be clamped to keep collision in the realm of possibilities.
	if (newRestitution > 1.f) {
		newRestitution = 1.f;
	}
	else if (newRestitution < 0.f) {
		newRestitution = 0.f;
	}
	restitutionCoefficient = newRestitution;
}

float cParticleContact::CalculateSeparatingVelocity() const
{
	glm::vec3 relativeVelocity = particle0->GetVelocity();

	// If a second particle exists, subtract its velocity.
	if (particle1)
	{
		relativeVelocity -= particle1->GetVelocity();
	}
	return glm::dot(relativeVelocity, contactNormal);
}

void cParticleContact::ResolveVelocity(float deltaTime)
{
	float separatingVelocity = CalculateSeparatingVelocity();

	if (separatingVelocity > 0.0f)
	{
		// Already separating. No need to resolve.
		return;
	}

	// calculate the new separating velocity (Newton's third law)
	float newSepVelocity = -separatingVelocity * restitutionCoefficient;

	// Check the velocity build-up due to accelaration only
	glm::vec3 velocityDueToAcceleration = particle0->GetAcceleration();
	if (particle1)
	{
		velocityDueToAcceleration -= particle1->GetAcceleration();
	}
	float accCausedSepVelocity = glm::dot(velocityDueToAcceleration, contactNormal) * deltaTime;

	// If we've got a closing velocity due to acceleration build-up
	// remove it from the new separating velocity
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitutionCoefficient * accCausedSepVelocity;

		// Make sure we haven't removed more than there was to remove.
		if (newSepVelocity < 0.0f)
		{
			newSepVelocity = 0.0f;
		}
	}

	float deltaVelocity = newSepVelocity - separatingVelocity;

	// We apply the change in velocity to each object in proportion to their
	// inverse mass (higher mass -> get less change in velocity)
	float totalInverseMass = particle0->GetInverseMass();
	if (particle1)
	{
		totalInverseMass += particle1->GetInverseMass();
	}

	// If all particles have infinite mass, then impulses have no effect
	if (totalInverseMass <= 0.0f)
	{
		return;
	}

	// Calculate the impulse to apply
	float impulse = deltaVelocity / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	glm::vec3 impulsePerIMass = contactNormal * impulse;

	// Apply impulses; The are applied in the direction of the contact
	// and they are proportional to the inverse mass
	particle0->SetVelocity(particle0->GetVelocity() + impulsePerIMass * particle0->GetInverseMass());

	if (particle1)
	{
		// Particle 1 will go in the opposite direction
		particle1->SetVelocity(particle1->GetVelocity() + impulsePerIMass * - particle1->GetInverseMass());
	}
}

void cParticleContact::ResolvePosition(float deltaTime)
{
	// If we don't have any penetration, skip this step
	if (penetrationDepth <= 0.f)
	{
		return;
	}

	float totalInverseMass = particle0->GetInverseMass();
	if (particle1)
	{
		totalInverseMass += particle1->GetInverseMass();
	}

	// If all particles have infinite mass, then we do nothing.
	if (totalInverseMass <= 0.f)
	{
		movement0 = glm::vec3(0.0f, 0.0f, 0.0f);
		movement1 = glm::vec3(0.0f, 0.0f, 0.0f);
		return;
	}

	// Find the amount of mPenetration resolution per unit of inverse mass
	// Contact normal is direction, penetrationDepth/totalInverseMass gives us magnitude
	glm::vec3 movePerIMass = contactNormal * (penetrationDepth / totalInverseMass);

	// Calculate the movement amounts
	movement0 = movePerIMass * particle0->GetInverseMass();
	if (particle1)
	{
		movement1 = movePerIMass * - particle1->GetInverseMass();
		particle1->SetPosition(particle1->GetPosition() + movement1);
	}
	else
	{
		// Clear movement1 for re-use.
		movement1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Apply the penetration resolution
	particle0->SetPosition(particle0->GetPosition() + movement0);
}