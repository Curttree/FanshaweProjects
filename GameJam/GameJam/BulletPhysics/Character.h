#pragma once
#include "../BulletDynamics/Character/btKinematicCharacterController.h"
#include <extern/glm/vec3.hpp>
#include <extern/glm/gtx/quaternion.hpp>
#include <Fanshawe2022Physics/interfaces/iCharacter.h>
#include <../BulletDynamics/Character/btKinematicCharacterController.h>

namespace gdp2022Physics
{
	class Character : public iCharacter, public btKinematicCharacterController {
	public:
		Character(btCollisionWorld* _world, btVector3 up);
		virtual void Initialize();
		virtual glm::vec3 GetPosition();
		virtual glm::quat GetRotation();
		virtual void TimeStep(float deltaTime);
	private:
		btCollisionWorld* world;
	};
}