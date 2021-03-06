#include "RigidBody.h"

#include <cassert>

#include "PhysicsConversion.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include "../BulletCollision/CollisionDispatch/btCollisionObject.h"

namespace gdp2022Physics
{
	RigidBody::RigidBody(const cRigidBodyDesc& desc, iShape* shape)
		: iRigidBody()
	{
		btQuaternion ori;
		btVector3 pos;
		btVector3 velo;

		CastBulletQuaternion(desc.rotation, &ori);
		CastBulletVector3(desc.position, &pos);
		CastBulletVector3(desc.linearVelocity, &velo);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(ori, pos));
		btVector3 inertia(0, 0, 0);
		btCollisionShape* bulletShape = CastBulletShape(shape);

		if (desc.mass != 0.f)
		{
			bulletShape->calculateLocalInertia(desc.mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo bodyCI(desc.mass, motionState, bulletShape, inertia);
		mBulletBody = new btRigidBody(bodyCI);

		mBulletBody->setActivationState(DISABLE_DEACTIVATION);

		if (glm::length(desc.linearVelocity) > 0.1f) {
			mBulletBody->applyImpulse(velo, btVector3(0, 0, 0));
		}
	}

	RigidBody::~RigidBody() {}

	bool RigidBody::IsStatic() const
	{
		return mBulletBody->isStaticObject();
	}

	iShape* RigidBody::GetShape()
	{
		return 0;
	}

	void RigidBody::GetPosition(glm::vec3& positionOut)
	{
		CastGLMVector3(mBulletBody->getCenterOfMassPosition(), &positionOut);
	}

	glm::vec3 RigidBody::GetPosition()
	{
		glm::vec3 positionOut;
		CastGLMVector3(mBulletBody->getCenterOfMassPosition(), &positionOut);
		return positionOut;
	}

	void RigidBody::SetPosition(const glm::vec3& positionIn)
	{
		// Can't do this.
		assert(0);
	}

	void RigidBody::GetOrientation(glm::quat& orientationOut)
	{
		CastGLMQuaternion(mBulletBody->getOrientation(), &orientationOut);
	}

	glm::quat RigidBody::GetOrientation()
	{
		glm::quat orientationOut;
		CastGLMQuaternion(mBulletBody->getOrientation(), &orientationOut);
		return orientationOut;
	}

	void RigidBody::SetOrientation(const glm::quat& orientationIn)
	{
	 	// Can't do this.
		assert(0);
	}

	void RigidBody::ApplyForce(const glm::vec3& force)
	{
		btVector3 btForce;
		CastBulletVector3(force, &btForce);
		mBulletBody->applyCentralForce(btForce);
	}
	void RigidBody::SetVelocity(const glm::vec3& velocity) {
		btVector3 btVelocity;
		CastBulletVector3(velocity, &btVelocity);
		mBulletBody->setLinearVelocity(btVelocity);

		//Velocity corrections. Ensure the object does not sink below the ground.
		if (mBulletBody->getCenterOfMassPosition().getY() > 0) {
			mBulletBody->setGravity(btVector3(0, -9.81, 0));
		}
		else if (mBulletBody->getCenterOfMassPosition().getY() < -1) {
			mBulletBody->setGravity(btVector3(0, 9.81, 0));
		}
		else {
			mBulletBody->setGravity(btVector3(0, 0, 0));
		}
	}

	void RigidBody::ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint)
	{
		btVector3 btForce;
		btVector3 btForceAtPoint;

		CastBulletVector3(force, &btForce);
		CastBulletVector3(relativePoint, &btForceAtPoint);

		mBulletBody->applyForce(btForce, btForceAtPoint);
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		btVector3 btImpulse;
		CastBulletVector3(impulse, &btImpulse);
		mBulletBody->applyCentralImpulse(btImpulse);
	}

	void RigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint)
	{
		btVector3 btImpulse;
		btVector3 btImpulseAtPoint;

		CastBulletVector3(impulse, &btImpulse);
		CastBulletVector3(relativePoint, &btImpulseAtPoint);

		mBulletBody->applyImpulse(btImpulse, btImpulseAtPoint);
	}

	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
		btVector3 btTorque;
		CastBulletVector3(torque, &btTorque);
		mBulletBody->applyTorque(btTorque);
	}

	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
	{
		btVector3 btTorqueImpulse;
		CastBulletVector3(torqueImpulse, &btTorqueImpulse);
		mBulletBody->applyTorque(btTorqueImpulse);
	}
}
