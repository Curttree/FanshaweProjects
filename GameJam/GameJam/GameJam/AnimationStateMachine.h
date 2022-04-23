#pragma once
#include "GameEvents.h"
#include "AnimationStates.h"
#include <map>
#include <vector>

class AnimationStateMachine : public iGameEventListener
{
public:
	AnimationStateMachine();
	~AnimationStateMachine();

	AnimationState GetCurrentState();
	AnimationState GetTransitioningState();

	bool GetTransitioning(Transition& transition, float& transitionFactor);
	bool GetTransitioning();

	void AddTransition(GameEvent* gameEvent, AnimationState from, AnimationState to,
		float duration, TransitionType type = TransitionType::Linear);

	virtual void Notify(GameEventType type, void* data);

	void Update(float dt);

private:
	void TransitionToState(Transition transition);

	bool FindTransitionToState(AnimationState from, GameEvent* gameEvent, Transition& out);

	std::map<AnimationState, std::vector<Transition>> stateTransitions;
	AnimationState currentState;
	AnimationState transitioningState;
	bool transitionInProgress;
	float factor;
	float currentTime;
	float transitionDuration;
};