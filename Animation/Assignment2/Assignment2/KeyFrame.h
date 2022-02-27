#pragma once

#include "Component.h"
#include "iCommand.h"
#include <extern/glm/vec3.hpp>
#include <extern/glm/gtx/quaternion.hpp>

enum EasingType
{
	EaseIn,
	EaseOut,
	EaseInOut,
	None
};

#define DIRECT_ROTATION 0
#define LERP_ROTATION 1
#define SLERP_ROTATION 2

struct KeyFramePosition : Component
{
	KeyFramePosition(float time, const glm::vec3& position, EasingType easingType = EasingType::None)
		: time(time), position(position), easingType(easingType) {}
	float time;
	glm::vec3 position;
	EasingType easingType;
};

struct KeyFrameScale : Component
{
	KeyFrameScale(float time, const float& scale, EasingType easingType = EasingType::None)
		: time(time), scale(scale), easingType(easingType) {}
	float time;
	float scale;
	EasingType easingType;
};

struct KeyFrameRotation : Component
{
	KeyFrameRotation(float time, const glm::quat& rotation, int interpolationType = 0, EasingType easingType = EasingType::None)
		: time(time), rotation(rotation), InterpolationType(interpolationType), easingType(easingType) {}
	float time;
	glm::quat rotation;
	EasingType easingType;
	int InterpolationType;
};

struct KeyFrameEvent : Component
{
	KeyFrameEvent(float time, iCommand* command, const bool& triggered = false)
		: time(time), command(command), triggered(triggered) {}
	float time;
	iCommand* command;
	bool triggered;
};