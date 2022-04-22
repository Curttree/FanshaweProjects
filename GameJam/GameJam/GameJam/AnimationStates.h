#pragma once
class GameEvent;

enum class AnimationState
{
	Shoot,
	Idle,
	Run,
	Walk,
	Waiting,
	Aim
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
