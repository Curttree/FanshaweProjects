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
Animation* AnimationSequence::PreviousAnimation() {
	// Shouldn't be called regularly, only if game speed has been reversed.
	// Note I am not setting the time to the end of execution here, as we may not always wish to do this.
	// If this is the only use case for this function, it can be added here.
	animations[currentIndex].currentTime = 0.f;
	currentIndex--;
	if (currentIndex < 0) {
		currentIndex = animations.size() - 1;
		if (repeat == false)
		{
			playing = false;
		}
	}
	return &animations[currentIndex];
}