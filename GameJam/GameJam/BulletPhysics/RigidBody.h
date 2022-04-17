#pragma once

#include <Fanshawe2022Physics/interfaces/iRigidBody.h>
#include <Fanshawe2022Physics/interfaces/cRigidBodyDesc.h>
#include <Fanshawe2022Physics/interfaces/iShape.h>

#include <btBulletDynamicsCommon.h>

namespace gdp2022Physics
{
	class RigidBody : public iRigidBody
	{
	public:
		RigidBody(const cRigidBodyDesc& desc, iShape* shape);
		virtual ~RigidBody();

		iShape* GetShape();

		virtual bool IsStatic() const;

		virtual void GetPosition(glm::vec3& positionOut);
		virtual glm::vec3 GetPosition();
		virtual void SetPosition(const glm::vec3& positionIn);

		virtual void GetOrientation(glm::quat& orientationOut);
		virtual glm::quat GetOrientation();
		virtual void SetOrientation(const glm::quat& orientationIn);

		virtual void ApplyForce(const glm::vec3& force);
		virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint);

		virtual void ApplyImpulse(const glm::vec3& impulse);
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint);

		virtual void ApplyTorque(const glm::vec3& torque);
		virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

		btRigidBody* GetBulletBody() { return mBulletBody; }
	private:
		btRigidBody* mBulletBody;

		RigidBody(const RigidBody&) { }
		RigidBody& operator=(const RigidBody&) { return *this; }
	};
}