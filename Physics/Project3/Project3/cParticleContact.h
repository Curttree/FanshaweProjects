#pragma once
#include "cParticle.h"
#include <extern_includes.h>

/// <summary>
/// Contains all the information about a collision that is required to resolve it.
/// </summary>
class cParticleContact
{
public:
	// The first (or only) particle involved in the contact.
	cParticle* particle0;
	// The second particle involved in the contact (could be null).
	cParticle* particle1;


	glm::vec3 contactNormal;
	float penetrationDepth;

	// The amount that mParticle0 is moved by during interpenetration resolution;
	// Do not set this when creating the contact.
	glm::vec3 movement0;
	// The amount that mParticle1 is moved by during interpenetration resolution.
	// Do not set this when creating the contact.
	glm::vec3 movement1;

	// Resolve this contact, for both velocity and interpenetration.
	void Resolve(float deltaTime);
	// Calculates the separating velocity at this contact.
	float CalculateSeparatingVelocity() const;
	// Handles impulse calculations for this collision.
	void ResolveVelocity(float deltaTime);
	// Handles interpenetration resolution for this collision.
	void ResolvePosition(float deltaTime);

	float GetRestitutionCoefficient();
	void SetRestitutionCoefficient(float newRestitution);
private:
	// The ratio of the final to initial relative speed between two objects after they collide. Ranges from 0 to 1 (1 being a perfectly elastic collision)
	float restitutionCoefficient;
};