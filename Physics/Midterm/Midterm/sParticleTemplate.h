#pragma once
// Class to store the configuration properties of the various projectiles.
// These values will be used to create particles. Default values are provided if not present in config.

struct sParticleTemplate {
	float size = 1.f;
	float damping = 1.f;
	float mass = 1.f;
	float muzzleVelocity = 25.f;
	float timeLimit = -1.f;
	float distanceLimit = -1.f;
	bool accelerate = false;
	bool applyGravity = true;
	glm::vec3 colour = glm::vec3(1.f,1.f,1.f);
};