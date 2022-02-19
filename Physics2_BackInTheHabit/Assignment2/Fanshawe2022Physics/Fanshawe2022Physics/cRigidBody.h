#pragma once

#include <Fanshawe2022Physics/interfaces/iRigidBody.h>
#include <Fanshawe2022Physics/interfaces/cRigidBodyDesc.h>
#include <Fanshawe2022Physics/interfaces/iShape.h>

#include "CollisionHandler.h"

namespace gdp2022Physics
{
	class cRigidBody : public iRigidBody
	{
	public:
		cRigidBody(const cRigidBodyDesc& desc, iShape* shape);
		~cRigidBody();

		iShape* GetShape();

		bool IsStatic() const;

		virtual void GetPosition(glm::vec3& positionOut);
		virtual glm::vec3 GetPosition();
		virtual void SetPosition(const glm::vec3& positionIn);

		virtual void GetOrientation(glm::quat& orientationOut);
		virtual glm::quat GetOrientation();
		virtual void SetOrientation(const glm::quat& orientationIn);

		virtual void ApplyForce(const glm::vec3& new_force);
		virtual void ApplyImpulse(const glm::vec3& impulse);

		virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint); 
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint);

		virtual void ApplyTorque(const glm::vec3& appliedTorque);
		virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

		virtual std::string GetFriendlyName();

		void SetGravityAcceleration(const glm::vec3& new_gravity);
		void UpdateAcceleration();

		void KillForces();
		void ApplyDamping(float deltaTime);
		void VerletStep1(float deltaTime);
		void VerletStep2(float deltaTime);
		void VerletStep3(float deltaTime);

		friend class CollisionHandler;
	private:
		bool isStatic;

		float mass;
		float invMass;

		glm::vec3 previousPosition;
		glm::vec3 position;
		glm::quat orientation;
		glm::vec3 linearVelocity;
		glm::vec3 angularVelocity;

		glm::vec3 linearAcceleration;
		glm::vec3 angularAcceleration;

		glm::vec3 gravity;

		iShape* shape;
		std::string friendlyName;

		glm::vec3 force;
		glm::vec3 torque;

		float linearDamping;
		float angularDamping;

		float restitution;
		float friction;

		cRigidBody(const cRigidBody&) { }
		cRigidBody& operator=(const cRigidBody&) { return *this; }
	};
}