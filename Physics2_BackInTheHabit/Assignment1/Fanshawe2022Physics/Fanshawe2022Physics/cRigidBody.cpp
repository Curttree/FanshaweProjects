#include "cRigidBody.h"

namespace gdp2022Physics
{
	cRigidBody::cRigidBody(const cRigidBodyDesc& desc, iShape* _shape)
		: iRigidBody()
		, shape(_shape)
		, mass(desc.mass)
		, isStatic(desc.isStatic)
		, position(desc.position)
		, velocity(desc.velocity)
		, damping(desc.damping)
		, restitution(desc.restitution)
		, friction(desc.friction)
	{
		if (isStatic || desc.mass <= 0.0f)
		{
			mass = 0.f;
			invMass = 0.f;
			isStatic;
		}
		else
		{
			mass = desc.mass;
			invMass = 1.0f / desc.mass;
		}
	}

	cRigidBody::~cRigidBody() {}

	bool cRigidBody::IsStatic() const
	{
		return isStatic;
	}

	iShape* cRigidBody::GetShape()
	{
		return shape;
	}

	void cRigidBody::GetPosition(glm::vec3& positionOut)
	{
		positionOut = position;
	}

	glm::vec3 cRigidBody::GetPosition()
	{
		return position;
	}

	void cRigidBody::SetPosition(const glm::vec3& positionIn)
	{
		position = positionIn;
	}

	void cRigidBody::GetOrientation(glm::quat& orientationOut)
	{
		orientationOut = orientation;
	}

	glm::quat cRigidBody::GetOrientation()
	{
		return orientation;
	}

	void cRigidBody::SetOrientation(const glm::quat& orientationIn)
	{
		orientation = orientationIn;
	}

	void cRigidBody::ApplyForce(const glm::vec3& new_force)
	{
		force += new_force;
	}

	void cRigidBody::SetGravityAcceleration(const glm::vec3& new_gravity)
	{
		gravity = new_gravity;
	}

	void cRigidBody::UpdateAcceleration()
	{
		if (isStatic)
			return;

		acceleration = force * invMass + gravity;
	}

	void cRigidBody::KillForces()
	{
		force = glm::vec3(0.0f);
	}

	void cRigidBody::ApplyDamping(float deltaTime)
	{
		velocity *= pow(1.f - damping, deltaTime);

		if (glm::length(velocity) < 0.001f) {
			velocity = glm::vec3(0.0f);
		}
	}

	void cRigidBody::VerletStep1(float deltaTime)
	{
		if (isStatic)
			return;

		previousPosition = position;
		position += (velocity + acceleration * (deltaTime * 0.5f)) * deltaTime;
	}

	void cRigidBody::VerletStep2(float deltaTime)
	{
		if (isStatic)
			return;

		velocity += acceleration * (deltaTime * 0.5f);
	}

	void cRigidBody::VerletStep3(float deltaTime)
	{
		VerletStep2(deltaTime);
	}
}
