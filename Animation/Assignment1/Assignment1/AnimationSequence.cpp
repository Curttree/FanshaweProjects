#include "AnimationSequence.h"

Animation* AnimationSequence::GetCurrent() {
	return &animations[currentIndex];
}
Animation* AnimationSequence::NextAnimation() {
	animations[currentIndex].currentTime = 0.f;
	currentIndex++;
	if (currentIndex >= animations.size()) {
		currentIndex = 0;
		if (repeat == false)
		{
			playing = false;
		}
	}
	return &animations[currentIndex];
}