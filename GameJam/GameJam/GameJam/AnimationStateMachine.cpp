#include "AnimationStateMachine.h"
#include <glm/glm.hpp>

AnimationStateMachine::AnimationStateMachine()
	: transitionInProgress(false)
	, currentState(AnimationState::Idle)
{

}

AnimationStateMachine::~AnimationStateMachine()
{
	stateTransitions.clear();
}

AnimationState AnimationStateMachine::GetCurrentState() {
	return currentState;
}

void AnimationStateMachine::Update(float dt)
{
	if (transitionInProgress)
	{
		currentTime += dt;
		currentTime = glm::clamp(currentTime, 0.f, transitionDuration);
		factor = currentTime / transitionDuration;

		printf("%1.2f | %1.2f\n", currentTime, transitionDuration);

		if (factor >= 1.f)
		{
			currentState = transitioningState;
			transitionInProgress = false;
			currentTime = 0.f;
			factor = 0.f;
			printf("Transition complete!\n");
		}
	}
}

void AnimationStateMachine::Notify(GameEventType type, void* data)
{
	Transition transition; 
	if (transitionInProgress && FindTransitionToState(transitioningState, (GameEvent*)data, transition)) {
		//A valid transition interrupt was found. Interrupt the transition.
		currentState = transitioningState;
		transitionInProgress = false;
		currentTime = 0.f;
		factor = 0.f;
	}
	if (FindTransitionToState(currentState, (GameEvent*)data, transition)) {
		TransitionToState(transition);
	}
}

void AnimationStateMachine::AddTransition(GameEvent* gameEvent, AnimationState from, AnimationState to,
	float duration, TransitionType type)
{
	if (gameEvent == 0)
		return;

	Transition transition;
	if (FindTransitionToState(from, gameEvent, transition))
		return;

	transition.gameEvent = gameEvent;
	transition.from = from;
	transition.to = to;
	transition.duration = duration;
	transition.type = type;

	stateTransitions[from].push_back(transition);
}

void AnimationStateMachine::TransitionToState(Transition transition)
{
	// We can either deny a transition request, if the state machine]
	// is currently in the progress of a transition.
	if (transitionInProgress)
		return;

	if (transition.duration <= 0.f)
	{
		printf("Transition completed immediately!\n");
		currentState = transition.to;
		return;
	}

	printf("Transition starting!\n");
	factor = 0;
	currentTime = 0.f;
	transitioningState = transition.to;
	transitionInProgress = true;
	transitionDuration = transition.duration;
}

bool AnimationStateMachine::FindTransitionToState(AnimationState from, GameEvent* gameEvent, Transition& out)
{
	std::map<AnimationState, std::vector<Transition>>::iterator itStateMap = stateTransitions.find(from);
	//auto itStateMap = std::find(m_StateTransitions.begin(), m_StateTransitions.end(), from);
	if (itStateMap == stateTransitions.end())
		return false;

	std::vector<Transition> transition_vec = itStateMap->second;
	for (unsigned int i = 0; i < transition_vec.size(); ++i)
	{
		if (transition_vec[i].gameEvent->GetType() == gameEvent->GetType())
		{
			if (transition_vec[i].gameEvent->CompareData(gameEvent))
			{
				out = transition_vec[i];
				return true;
			}
		}
	}
	return false;
}