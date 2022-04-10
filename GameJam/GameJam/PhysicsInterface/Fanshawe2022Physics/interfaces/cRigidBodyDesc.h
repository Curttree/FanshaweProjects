#pragma once

#include <extern/glm/glm.hpp>

namespace gdp2022Physics
{
	class cRigidBodyDesc
	{
	public:
		cRigidBodyDesc()
			: isStatic(false)
			, position(glm::vec3(0.f))
			, velocity(glm::vec3(0.f))
			, mass(1.f)
			, damping (0.f)
			, restitution(0.75f)
			, friction(0.95f) {};

		bool isStatic;

		glm::vec3 position;
		glm::vec3 velocity;

		float mass;
		float damping;
		float restitution;
		float friction;
	};
}