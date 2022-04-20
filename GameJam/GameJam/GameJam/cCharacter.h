#pragma once
#include <cEntity.h>
#include "GameEvents.h"
#include "AnimationStateMachine.h"
#include <Fanshawe2022Physics/interfaces/iCharacter.h>


class cCharacter : public cEntity, public iGameEventListener {
public:
	cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation);

	//via cEntity
	virtual void TimeStep(float deltaTime);

	//via iGameEventListener
	virtual void Notify(GameEventType type, void* data);

	virtual AnimationState GetAnimationState(void);
	virtual std::string GetAnimationStateAsString(void);
	virtual void SetNeutralOrientation();
	virtual void ResetToNeutralOrientation();
private:
	iCharacter* physicsCharacter;
	AnimationStateMachine animationStateMachine;

	float height = 1.f;
	float diameter = 1.f;

	float idleTimer = 0.f;
	float maxIdleTime = 10.f;
	bool isIdle;
	glm::quat neutralOrientation;

	void BuildAnimationTransitions(void);

	//Temp map for debugging.
	std::map<AnimationState, std::string> states;
};