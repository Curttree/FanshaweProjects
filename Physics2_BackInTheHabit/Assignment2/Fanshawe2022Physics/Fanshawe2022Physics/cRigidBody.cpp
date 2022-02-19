#include "cRigidBody.h"
#include <iostream>
namespace gdp2022Physics
{
	cRigidBody::cRigidBody(const cRigidBodyDesc& desc, iShape* _shape)
		: iRigidBody()
		, shape(_shape)
		, mass(desc.mass)
		, isStatic(desc.isStatic)
		, position(desc.position)
		, linearVelocity(desc.linearVelocity)
		, linearDamping(desc.linearDamping)
		, restitution(desc.restitution)
		, friction(desc.friction)
		, friendlyName(desc.friendlyName)
		, orientation(desc.orientation)
		, angularVelocity(desc.angularVelocity)
		, angularDamping(desc.angularDamping)
		, linearAcceleration(glm::vec3(0.f))
		, angularAcceleration(glm::vec3(0.f))
		, force(glm::vec3(0.f))
		, gravity(glm::vec3(0.f))
		, torque(glm::vec3(0.f))

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
	
	void cRigidBody::ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint)
	{
		ApplyForce(force);
		ApplyTorque(glm::cross(relativePoint, force));
	}

	void cRigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		linearVelocity += impulse * invMass;
	}

	void cRigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint)
	{
		ApplyTorqueImpulse(glm::cross(relativePoint, impulse));
	}

	void cRigidBody::ApplyTorque(const glm::vec3& appliedTorque)
	{
		torque += appliedTorque;
	}

	void cRigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
	{
		angularVelocity += torqueImpulse;
	}

	void cRigidBody::SetGravityAcceleration(const glm::vec3& new_gravity)
	{
		gravity = new_gravity;
	}

	void cRigidBody::UpdateAcceleration()
	{
		if (isStatic)
			return;

		linearAcceleration = force * invMass + gravity;
		angularAcceleration = torque;
	}

	void cRigidBody::KillForces()
	{
		force = glm::vec3(0.0f);
		torque = glm::vec3(0.0f);
	}

	void cRigidBody::ApplyDamping(float deltaTime)
	{
		linearVelocity *= pow(1.f - linearDamping, deltaTime);
		angularVelocity *= pow(1.f - angularDamping, deltaTime);

		if (glm::length(linearVelocity) < 0.001f) {
			linearVelocity = glm::vec3(0.0f);
		}
	}

	void cRigidBody::VerletStep1(float deltaTime)
	{
		if (isStatic)
			return;

		previousPosition = position;
		position += (linearVelocity + linearAcceleration * (deltaTime * 0.5f)) * deltaTime;

		glm::vec3 axis = (angularVelocity + angularAcceleration * (deltaTime * 0.5f)) * deltaTime;
		float angle = glm::length(axis);
		axis = glm::normalize(axis);
		if (angle != 0.f)
		{
			glm::quat rot = glm::angleAxis(angle, axis);
			orientation *= rot;
		}
	}

	void cRigidBody::VerletStep2(float deltaTime)
	{
		if (isStatic)
			return;

		linearVelocity += linearAcceleration * (deltaTime * 0.5f);

		angularVelocity += angularAcceleration * (deltaTime * 0.5f);
	}

	void cRigidBody::VerletStep3(float deltaTime)
	{
		VerletStep2(deltaTime);
	}

	std::string cRigidBody::GetFriendlyName() {
		return friendlyName;
	}
}
