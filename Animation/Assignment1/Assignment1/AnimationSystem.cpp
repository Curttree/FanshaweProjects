#include "AnimationSystem.h"

#include "Animation.h"

#include <extern/glm/gtx/easing.hpp>
#include <iostream>

void AnimationSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	Animation* animationPtr;
	cEntity* currentEntityPtr;

	for (int i = 0; i < entities.size(); ++i)
	{
		currentEntityPtr = entities[i];

		animationPtr = &currentEntityPtr->animation;

		if (animationPtr == 0)
			continue;

		if (!animationPtr->playing)
			continue;

		float newTime = animationPtr->currentTime + dt * animationPtr->speed;

		// Handle Events
		if (animationPtr->keyFrameEvents.size() > 0) {
			int currEventFrameidx = -1;
			if (dt * animationPtr->speed >= 0.f) {
				currEventFrameidx = FindKeyFrameEventIndex(animationPtr, animationPtr->currentTime, newTime);
			}
			else {
				currEventFrameidx = FindKeyFrameEventIndex(animationPtr, newTime, animationPtr->currentTime);
			}
			if (currEventFrameidx >= 0) {
				//TODO : Callback function.
				std::cout << animationPtr->keyFrameEvents[currEventFrameidx].message << std::endl;
			}
		}

		animationPtr->currentTime = newTime;

		if (animationPtr->currentTime > animationPtr->duration)
		{
			animationPtr->currentTime = animationPtr->duration;
			if (animationPtr->repeat == true)
			{
				animationPtr->currentTime = 0;
			}
			else
			{
				animationPtr->playing = false;
			}


		}

		// Find active keyframes
		if (animationPtr->keyFramePositions.size() == 0
			|| animationPtr->keyFrameScales.size() == 0
			|| animationPtr->keyFrameRotations.size() == 0)
		{
			printf("Contains an empty keyframe vector");
			continue;
		}

		// Currently we are using only the current KeyFrame,
		// but we need to smoothly animate between two KeyFrames
		// If the current time is at 1.5, and two KeyFrames are at
		// 1, and 2, then we must find a point between the two 
		// positions
		// 
		//KeyFrame keyframe = FindCurrentKeyFrame(animationPtr, animationPtr->currentTime);
		//positionPtr->value = keyframe.position;

		// If there is only 1 KeyFrame in the animation, only use that
		if (animationPtr->keyFramePositions.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->position = animationPtr->keyFramePositions[0].position;
			return;
		}

		int currPosFrameidx = FindKeyFramePositionIndex(animationPtr, animationPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currPosFrameidx == animationPtr->keyFramePositions.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->position = animationPtr->keyFramePositions[currPosFrameidx].position;
			return;
		}

		/* UPDATE POSITIONS */

		// Find a position between the current KeyFrame, and the next KeyFrame
		int nextPosFrameidx = currPosFrameidx + 1;

		const KeyFramePosition& keyFramePos1 = animationPtr->keyFramePositions[currPosFrameidx];
		const KeyFramePosition& keyFramePos2 = animationPtr->keyFramePositions[nextPosFrameidx];

		// Look into glm's easing functions
		// 1. Ease-In
		// 2. Ease-Out
		// 3. Ease-InOut
		// glm::sineEaseIn()


		// As the animation time 
		// How would I change this to implement glm::sineEaseIn(x)?
		float posFraction = (animationPtr->currentTime - keyFramePos1.time) / (keyFramePos2.time - keyFramePos1.time);

		switch (keyFramePos2.easingType)
		{
		case EaseIn:
			posFraction = glm::sineEaseIn(posFraction);
			break;
		case EaseOut:
			posFraction = glm::sineEaseInOut(posFraction);
			break;
		case EaseInOut:
			posFraction = glm::sineEaseInOut(posFraction);
			break;
		default:
			break;
		}
		currentEntityPtr->position = keyFramePos1.position + (keyFramePos2.position - keyFramePos1.position) * posFraction;

		//printf("KeyFrame(%d -> %.2f -> %d) position: (%.2f, %.2f)\n",
		//	CurrentKeyFrameIndex, fraction, NextKeyFrameIndex, positionPtr->value.x, positionPtr->value.y);

		//positionPtr->value = (coords1 + coords2) / 2.f * animationPtr->currentTime;

		/* UPDATE SCALES*/

		if (animationPtr->keyFrameScales.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->scale = animationPtr->keyFrameScales[0].scale;
			return;
		}

		int currScaleFrameidx = FindKeyFrameScaleIndex(animationPtr, animationPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currScaleFrameidx == animationPtr->keyFrameScales.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->scale = animationPtr->keyFrameScales[currScaleFrameidx].scale;
			return;
		}

		int nextScaleFrameidx = currScaleFrameidx + 1;

		const KeyFrameScale& keyFrameScale1 = animationPtr->keyFrameScales[currScaleFrameidx];
		const KeyFrameScale& keyFrameScale2 = animationPtr->keyFrameScales[nextScaleFrameidx];

		float scaleFraction = (animationPtr->currentTime - keyFrameScale1.time) / (keyFrameScale2.time - keyFrameScale1.time);

		switch (keyFrameScale2.easingType)
		{
		case EaseIn:
			scaleFraction = glm::sineEaseIn(scaleFraction);
			break;
		case EaseOut:
			scaleFraction = glm::sineEaseInOut(scaleFraction);
			break;
		case EaseInOut:
			scaleFraction = glm::sineEaseInOut(scaleFraction);
			break;
		default:
			break;
		}
		currentEntityPtr->scale = keyFrameScale1.scale + (keyFrameScale2.scale - keyFrameScale1.scale) * scaleFraction;


		/* UPDATE ROTATIONS*/

		if (animationPtr->keyFrameRotations.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->rotation = animationPtr->keyFrameRotations[0].rotation;
			return;
		}

		int currRotFrameidx = FindKeyFrameRotIndex(animationPtr, animationPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currRotFrameidx == animationPtr->keyFrameRotations.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->rotation = animationPtr->keyFrameRotations[currRotFrameidx].rotation;
			return;
		}

		int nextRotFrameidx = currRotFrameidx + 1;

		const KeyFrameRotation& keyFrameRot1 = animationPtr->keyFrameRotations[currRotFrameidx];
		const KeyFrameRotation& keyFrameRot2 = animationPtr->keyFrameRotations[nextRotFrameidx];

		float rotationFraction = (animationPtr->currentTime - keyFrameRot1.time) / (keyFrameRot2.time - keyFrameRot1.time);

		switch (keyFrameRot2.easingType)
		{
		case EaseIn:
			rotationFraction = glm::sineEaseIn(rotationFraction);
			break;
		case EaseOut:
			rotationFraction = glm::sineEaseInOut(rotationFraction);
			break;
		case EaseInOut:
			rotationFraction = glm::sineEaseInOut(rotationFraction);
			break;
		default:
			break;
		}

		if (keyFrameRot2.InterpolationType == 1)
		{
			currentEntityPtr->rotation = glm::lerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction);
		}
		if (keyFrameRot2.InterpolationType == 2)
		{
			currentEntityPtr->rotation = glm::slerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction);
		}
		else
		{
			currentEntityPtr->rotation = keyFrameRot1.rotation + (keyFrameRot2.rotation - keyFrameRot1.rotation) * rotationFraction;
		}
	}
}

int AnimationSystem::FindKeyFramePositionIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFramePositions.size(); ++keyFrameIndex)
	{
		if (animation->keyFramePositions[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFramePositions.size() - 1;
}

int AnimationSystem::FindKeyFrameScaleIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFrameScales.size(); ++keyFrameIndex)
	{
		if (animation->keyFrameScales[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFrameScales.size() - 1;
}

int AnimationSystem::FindKeyFrameRotIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFrameRotations.size(); ++keyFrameIndex)
	{
		if (animation->keyFrameRotations[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFrameRotations.size() - 1;
}

int AnimationSystem::FindKeyFrameEventIndex(Animation* animation, float old_time, float new_time )
{
	int keyFrameIndex = 0;
	for (; keyFrameIndex < animation->keyFrameEvents.size(); ++keyFrameIndex)
	{
		// Slightly different comparison logic to support multiple events at the same time step.
		if (animation->keyFrameEvents[keyFrameIndex].time > old_time && animation->keyFrameEvents[keyFrameIndex].time <= new_time)
			return keyFrameIndex;
	}

	// No events for this frame
	return -1;
}