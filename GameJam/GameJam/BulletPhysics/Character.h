#pragma once
#include "../BulletDynamics/Character/btKinematicCharacterController.h"
#include <extern/glm/vec3.hpp>
#include <extern/glm/gtx/quaternion.hpp>
#include <Fanshawe2022Physics/interfaces/iCharacter.h>

namespace gdp2022Physics
{
	class Character : public iCharacter {
	public:
		virtual void Initialize();
		virtual glm::vec3 GetPosition();
		virtual glm::quat GetRotation();
	private:
		btPairCachingGhostObject* ghostObject;
		btKinematicCharacterController* bulletController;
	};
}