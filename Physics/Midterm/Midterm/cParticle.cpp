#include "cParticle.h"

cParticle::cParticle() {

}

cParticle::~cParticle() {

}

cParticle::cParticle(float _mass, glm::vec3& _position) {
	position = _position;
	mass = _mass;
}

#pragma region Position
glm::vec3 cParticle::GetPosition() {
	return position;
}

void cParticle::SetPosition(const glm::vec3 newPosition) {
	 position = newPosition;
}
#pragma endregion

#pragma region Velocity
glm::vec3 cParticle::GetVelocity() {
	return velocity;
}

void cParticle::SetVelocity(const glm::vec3 newVelocity) {
	velocity = newVelocity;
}
#pragma endregion

#pragma region Acceleration
glm::vec3 cParticle::GetAcceleration() {
	return acceleration;
}

void cParticle::SetAcceleration(const glm::vec3 newAcceleration) {
	acceleration = newAcceleration;
}
#pragma endregion

#pragma region Mass
float cParticle::GetMass() {
	return mass;
}

void cParticle::SetMass(float newMass) {
	// Not sure if this would be useful for particles, but keeping it here in case this class gets expanded
	// More complex objects may change mass due to external factors (ex. character puts on heavier armour).
	mass = newMass;
}
#pragma endregion

#pragma region NetAppliedForce
glm::vec3 cParticle::GetAppliedForce() {
	return netAppliedForce;
}

void cParticle::SetAppliedForce(const glm::vec3 newForce) {
	netAppliedForce = newForce;
}
#pragma endregion

#pragma region Damping
float cParticle::GetDamping() {
	return damping;
}

void cParticle::SetDamping(float newDamping) {
	damping = glm::clamp(newDamping, 0.f, 1.f);
}
#pragma endregion

glm::vec3 cParticle::ApplyForce(const glm::vec3 force)
{
	netAppliedForce += force;
	return netAppliedForce;
}

void cParticle::Integrate(float deltaTime)
{
	if (mass <= 0.f)
	{
		return; // mass is less than or equal 0. Consider to be static.
	}

	// Time step
	position += velocity * deltaTime;	
	velocity += (acceleration + netAppliedForce * 1.f / mass) * deltaTime;

	// apply damping
	velocity *= glm::pow(damping, deltaTime);

	// clear applied forces
	SetAppliedForce(glm::vec3(0.f));
}