#pragma once
#include <cEntity.h>
#include "GameEvents.h"
#include "AnimationStateMachine.h"
#include <Fanshawe2022Physics/interfaces/iCharacter.h>
#include "cPlayerProxy.h"
#include "cParticleEmitter.h"


class cCharacter : public cEntity, public iGameEventListener {
public:
	cCharacter(glm::vec3 startPosition, glm::vec3 startOrientation);

	//via cEntity
	virtual void TimeStep(float deltaTime);

	//via iGameEventListener
	virtual void Notify(GameEventType type, void* data);

	virtual AnimationState GetAnimationState(void);
	virtual AnimationState GetFutureAnimationState(void);
	virtual std::string GetAnimationStateAsString(void);
	virtual void SetNeutralOrientation();
	virtual void ResetToNeutralOrientation();
	virtual void LoadBones();
	virtual void LoadAnimation();
	virtual bool isRunning();
	cPlayerProxy* physicsProxy;
private:
	iCharacter* physicsCharacter;
	AnimationStateMachine animationStateMachine;
	cParticleEmitter particleEmitter;

	float height = 1.f;
	float diameter = 1.f;

	float idleTimer = 0.f;
	float maxIdleTime = 15.f;
	float stoppingTimer = 0.f;
	float maxStoppingTime = 0.5f;
	bool isIdle;
	bool isStopping;
	glm::vec3 proxyOffset;
	glm::quat neutralOrientation;

	Animation* current_animation;
	cBoneHierarchy* current_bones;

	std::map<AnimationState, std::string> map_animations;

	void BuildAnimationTransitions(void);
	void BuildAnimationMap(void);
	std::string GetAnimationFromState(AnimationState state);

	void UpdateAnimationBlend(void);

	//Gets the position to draw the gun. Returns false if it should not be drawn.
	bool GetGunPosition(glm::vec3& position);

	//Temp map for debugging.
	std::map<AnimationState, std::string> states;
};