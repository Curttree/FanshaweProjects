#pragma once
#include "GameEvents.h"

enum class AnimationState
{
	Shoot,
	Idle,
	Swim,
	Run,
	Walk,
	Waiting
};

enum class TransitionType
{
	Linear
};

struct Transition
{
	GameEvent* gameEvent;
	AnimationState from;
	AnimationState to;
	float duration;
	TransitionType type;
};
