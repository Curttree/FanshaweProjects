#pragma once
// Class to store the configuration properties of the various projectiles.
// These values will be used to create particles.

//TODO: Consider setting defualt values either here or in config file.
struct sParticleTemplate {
	float size;
	float damping;
	float mass;
	float muzzleVelocity;
	float timeLimit;
	float distanceLimit;
};