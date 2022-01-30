#pragma once

#include "System.h"

#include "KeyFrame.h"
#include "Animation.h"

class AnimationSystem : System
{
public:
	virtual void Process(const std::vector<cEntity*>& entities, float dt);
private:
	//const KeyFramePosition& FindCurrentKeyFramePosition(Animation* animation, float time);
	//const KeyFrameScale& FindCurrentKeyFrameScale(Animation* animation, float time);
	int FindKeyFramePositionIndex(Animation* animation, float time);
	int FindKeyFrameScaleIndex(Animation* animation, float time);
	int FindKeyFrameRotIndex(Animation* animation, float time);
	int FindKeyFrameEventIndex(Animation* animation, float old_time, float new_time);
};