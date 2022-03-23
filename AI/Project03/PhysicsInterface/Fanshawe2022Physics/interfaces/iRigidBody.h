#pragma once

#include <extern/glm/glm.hpp>
#include <extern/glm/gtx/quaternion.hpp>

namespace gdp2022Physics
{
	class iRigidBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual void GetPosition(glm::vec3& positionOut) = 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual void SetPosition(const glm::vec3& positionIn) = 0;

		virtual void GetOrientation(glm::quat& orientationOut) = 0;
		virtual glm::quat GetOrientation() = 0;
		virtual void SetOrientation(const glm::quat& orientationIn) = 0;

		virtual void ApplyForce(const glm::vec3& force) = 0;

	protected:
		iRigidBody() {}

	private:
		iRigidBody(const iRigidBody&) {}
		iRigidBody& operator=(const iRigidBody&) { return *this; }
	};
}