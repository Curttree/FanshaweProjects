#pragma once
#include <vector>
#include "Animation.h"

class AnimationSequence {
public:
	bool playing;
	float speed;
	bool repeat;
	std::vector<Animation> animations;

	Animation* GetCurrent();
	Animation* NextAnimation();

private:
	int currentIndex;
};