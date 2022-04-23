#include "Character.h"
#include "PhysicsConversion.h"
#include <../BulletCollision/CollisionDispatch/btGhostObject.h>

namespace gdp2022Physics
{
	Character::Character(btCollisionWorld* _world, btPairCachingGhostObject* ghost, btVector3 up) : btKinematicCharacterController(ghost,new btCapsuleShape(1.75f,1.75f),1.f,up), world(_world){
		setUseGhostSweepTest(false);

	}
	void Character::Initialize() {
		btVector3 speed;
		btScalar time;
		CastBulletVector3(glm::vec3(0.f, 0.f, 1.f), &speed);
		CastBulletScalar(1.f, &time);
		setVelocityForTimeInterval(speed, time);
	}

	glm::vec3 Character::GetPosition() {
		return glm::vec3(0.f);
	}
	glm::quat Character::GetRotation() {
		return glm::quat(glm::vec3(0.f));
	}
	void Character::TimeStep(float deltaTime) {
		updateAction(world, deltaTime);
	}
}