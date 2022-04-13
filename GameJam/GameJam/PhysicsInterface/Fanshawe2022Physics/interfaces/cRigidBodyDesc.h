#pragma once
#include <string>
#include <extern/glm/gtx/quaternion.hpp>
#include <extern/glm/glm.hpp>

namespace gdp2022Physics
{
	class cRigidBodyDesc
	{
	public:
		cRigidBodyDesc()
			: isStatic(false)
			, position(glm::vec3(0.f))
			, rotation(glm::quat(glm::vec3(0.f)))
			, linearVelocity(glm::vec3(0.f))
			, angularVelocity(glm::vec3(0.f))
			, mass(1.f)
			, linearDamping(0.f)
			, angularDamping(0.f)
			, restitution(0.75f)
			, friction(0.95f) {};

		bool isStatic;

		glm::vec3 position;
		glm::vec3 linearVelocity;
		glm::vec3 angularVelocity;

		float mass;
		float linearDamping;
		float angularDamping;
		float restitution;
		float friction;

		glm::quat rotation;

		std::string friendlyName;
	};
}