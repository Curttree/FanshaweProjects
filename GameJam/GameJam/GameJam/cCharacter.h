#pragma once
#include <cEntity.h>
#include "GameEvents.h"
#include "AnimationStateMachine.h"

//Forward declaration of Bullet classes. Only pointers are used in the header, so no need to reference actual classes quite yet.
class btPairCachingGhostObject;
class btKinematicCharacterController;


class cCharacter : public cEntity, public iGameEventListener {
public:
	cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation);

	//via cEntity
	virtual void TimeStep(float deltaTime);

	//via iGameEventListener
	virtual void Notify(GameEventType type, void* data);

	virtual AnimationState GetAnimationState(void);
	virtual std::string GetAnimationStateAsString(void);
private:
	btPairCachingGhostObject* ghostObject;
	btKinematicCharacterController* bulletController;
	AnimationStateMachine animationStateMachine;

	float height = 1.f;
	float diameter = 1.f;

	float idleTimer = 0.f;
	float maxIdleTime = 10.f;
	bool isIdle;

	void BuildAnimationTransitions(void);

	//Temp map for debugging.
	std::map<AnimationState, std::string> states;
};