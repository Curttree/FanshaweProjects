#include "cParticle.h"
#include <extern/glm/gtc/matrix_transform.hpp>

cParticle::cParticle() {

}

cParticle::~cParticle() {

}

cParticle::cParticle(glm::vec3& _position, float _density, float _radius) {
	position = _position;
	density = _density;
	radius = _radius;
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
	// Calculation for mass of sphere given radius and density.
	return density * (4.f / 3.f * glm::pi<float>() * glm::pow(radius,3.f));
}

float cParticle::GetInverseMass() {
	return 1.f / GetMass();
}

#pragma endregion

#pragma region Radius
float cParticle::GetRadius() {
	return radius;
}

void cParticle::SetRadius(float newRadius) {
	if (newRadius < 0.f) {
		// Cannot have inverse mass. Set lower limit to 0.f (static object).
		newRadius = 0.f;
	}
	radius = newRadius;
}
#pragma endregion

#pragma region Density
float cParticle::GetDensity() {
	return density;
}

void cParticle::SetDensity(float newDensity) {
	if (newDensity < 0.f) {
		// Cannot have inverse mass. Set lower limit to 0.f (static object).
		newDensity = 0.f;
	}
	density = newDensity;
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

glm::vec3 cParticle::ApplyImpulse(const glm::vec3 impulse)
{
	velocity += impulse * GetInverseMass();
	return velocity;
}

void cParticle::Integrate(float deltaTime)
{
	if (GetMass() <= 0.f)
	{
		return; // mass is less than or equal 0. Consider to be static.
	}

	// Time step
	position += velocity * deltaTime;	
	velocity += (acceleration + netAppliedForce * GetInverseMass()) * deltaTime;

	// apply damping
	velocity *= glm::pow(damping, deltaTime);

	// clear applied forces
	SetAppliedForce(glm::vec3(0.f));
}