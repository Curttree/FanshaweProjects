#include "Character.h"
#include "PhysicsConversion.h"
#include <../BulletCollision/CollisionDispatch/btGhostObject.h>

namespace gdp2022Physics
{
	Character::Character(btCollisionWorld* _world, btVector3 up) : btKinematicCharacterController(new btPairCachingGhostObject(),new btCapsuleShape(1.f,1.f),1.f,up), world(_world){

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
		playerStep(world, deltaTime);
	}
}