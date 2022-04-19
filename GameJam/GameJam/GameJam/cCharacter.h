#pragma once
#include <cEntity.h>

//Forward declaration of Bullet classes. Only pointers are used in the header, so no need to reference actual classes quite yet.
class btPairCachingGhostObject;
class btKinematicCharacterController;


class cCharacter : public cEntity {
public:
	cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation);
	virtual void TimeStep(float deltaTime);
private:
	btPairCachingGhostObject* ghostObject;
	btKinematicCharacterController* bulletController;
	float height = 1.f;
	float diameter = 1.f;
};