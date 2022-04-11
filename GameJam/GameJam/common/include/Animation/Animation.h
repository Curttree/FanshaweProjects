#pragma once

#include <vector>
#include <string>

#include "Component.h"
#include "KeyFrame.h"

struct AnimationNode
{
	std::string name;
	glm::mat4 transformation;
	std::vector<KeyFramePosition> keyFramePositions;
	std::vector<KeyFrameScale> keyFrameScales;
	std::vector<KeyFrameRotation> keyFrameRotations;
	std::vector<AnimationNode*> children;
};

struct Animation : Component
{
	bool shouldPlay;
	float duration;
	float currentTime;
	float speed;
	bool repeat;
	std::string name;
	unsigned int numTicks;
	unsigned int ticksPerSecond;
	std::vector<KeyFramePosition> keyFramePositions;
	std::vector<KeyFrameScale> keyFrameScales;
	std::vector<KeyFrameRotation> keyFrameRotations;
	std::vector<KeyFrameEvent> keyFrameEvents;
	std::vector<AnimationNode*> animationNodes;
};

struct AnimationBlend : Component
{
	Animation* anim1;
	Animation* anim2;
	float factor;
	float speed;
	float direction;
};
