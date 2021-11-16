#pragma once
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/exponential.hpp>

class cParticle {
public:
	cParticle();
	cParticle(float _mass, glm::vec3& _position);
	virtual ~cParticle();

	glm::vec3 GetPosition();
	void SetPosition(const glm::vec3 newPosition);

	glm::vec3 GetVelocity();
	void SetVelocity(const glm::vec3 newVelocity);

	glm::vec3 GetAcceleration();
	void SetAcceleration(const glm::vec3 newAcceleration);

	float GetMass();
	float GetInverseMass();
	void SetMass(float newMass);

	glm::vec3 GetAppliedForce();
	void SetAppliedForce(const glm::vec3 newForce);

	float GetDamping();
	void SetDamping(float newDamping);

	// Returns the new net applied force.
	glm::vec3 ApplyForce(const glm::vec3 force);

	// Returns the new velocity
	glm::vec3 ApplyImpulse(const glm::vec3 impulse);

	void Integrate(float deltaTime);
private:
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 velocity = glm::vec3(0.f);
	glm::vec3 acceleration = glm::vec3(0.f);
	float mass = 1.f;
	glm::vec3 netAppliedForce = glm::vec3(0.f);
	float damping = 0.99f;
};