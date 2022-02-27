#pragma once

#include <vector>

#include "Component.h"
#include "KeyFrame.h"

struct Animation : Component
{
	bool shouldPlay;
	float duration;
	float currentTime;
	float speed;
	bool repeat;
	std::vector<KeyFramePosition> keyFramePositions;
	std::vector<KeyFrameScale> keyFrameScales;
	std::vector<KeyFrameRotation> keyFrameRotations;
	std::vector<KeyFrameEvent> keyFrameEvents;
};