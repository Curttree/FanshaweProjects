#pragma once

#include <extern/glm/glm.hpp>
#include <extern/glm/gtx/quaternion.hpp>
#include "eBodyType.h"
#include "iCollisionBody.h"

namespace gdp2022Physics
{
	class iRigidBody : public iCollisionBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual void GetPosition(glm::vec3& positionOut) = 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual void SetPosition(const glm::vec3& positionIn) = 0;

		virtual void GetOrientation(glm::quat& orientationOut) = 0;
		virtual glm::quat GetOrientation() = 0;
		virtual void SetOrientation(const glm::quat& orientationIn) = 0;

		virtual void ApplyImpulse(const glm::vec3& impulse) = 0;
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint) = 0;

		virtual void ApplyForce(const glm::vec3& force) = 0;

		virtual bool IsStatic() const = 0;

	protected:
		iRigidBody() :  iCollisionBody(RIGID_BODY) {}

	private:
		iRigidBody(const iRigidBody&) : iCollisionBody(RIGID_BODY) {}
		iRigidBody& operator=(const iRigidBody&) { return *this; }
	};
}