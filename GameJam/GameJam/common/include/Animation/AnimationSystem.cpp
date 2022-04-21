#include "AnimationSystem.h"

#include "Animation.h"

#include <extern/glm/gtx/easing.hpp>
#include <iostream>

void AnimationSystem::Process(const std::vector<cEntity*>& entities, float dt) {
	cEntity* currentEntityPtr = 0;
	Animation* animationPtr = 0;
	AnimationBlend* animationBlendPtr = 0;

	for (int i = 0; i < entities.size(); ++i)
	{
		currentEntityPtr = entities[i];

		if (currentEntityPtr->animationBlend.anim1 != nullptr && currentEntityPtr->animationBlend.anim2 != nullptr) {
			//BlendAnimationUpdate(currentEntityPtr, dt);
		}
		else {
			SingleAnimationUpdate(currentEntityPtr, dt);
		}
	}
}

void AnimationSystem::UpdateAnimationTime(Animation* animation, float dt) {
	if (!animation->shouldPlay)
		return;

	animation->currentTime += dt * animation->speed;

	while (animation->currentTime > animation->duration)
		animation->currentTime -= animation->duration;
}

void AnimationSystem::SingleAnimationUpdate(cEntity* pEntity, float dt)
{
	Animation* animationPtr = &pEntity->animation;

	if (!animationPtr->shouldPlay)
		return;

	UpdateAnimationTime(animationPtr, dt);

	std::vector<glm::mat4> transforms;
	std::vector<glm::mat4> globals;
	std::vector<glm::mat4> offsets;


	// Since we only have a single animated character using this animation
	// We don't need to do anything with the transforms, globals, and offsets.
	// Since we are grabbing the data directly from the MeshData in RenderSystem
	// as-well.

	// Test a specific time

	pEntity->mesh->meshData->UpdateTransforms(animationPtr->currentTime, animationPtr, transforms, globals, offsets);

	return;
}
void AnimationSystem::Process_Old(const std::vector<cEntity*>& entities, float dt)
{
	Animation* animPtr;
	AnimationSequence* animSequencePtr;
	cEntity* currentEntityPtr;

	for (int i = 0; i < entities.size(); ++i)
	{
		currentEntityPtr = entities[i];
		animSequencePtr = &currentEntityPtr->animations;
		animPtr = animSequencePtr->GetCurrent();

		if (animPtr == 0)
			continue;

		if (!animSequencePtr->playing)
			continue;

		if (!animPtr->shouldPlay) {
			// Cycle to the next animation (to be re-evaluated next frame
			animPtr = animSequencePtr->NextAnimation();
			continue;
		}

		float newTime = animPtr->currentTime + dt * animPtr->speed * animSequencePtr->speed;

		// Handle Events
		// Note: We are currently limited to one event per animation/iteration.
		if (animPtr->keyFrameEvents.size() > 0) {
			int currEventFrameidx = -1;
			if (dt * animPtr->speed >= 0.f) {
				currEventFrameidx = FindKeyFrameEventIndex(animPtr, animPtr->currentTime, newTime);
			}
			else {
				currEventFrameidx = FindKeyFrameEventIndex(animPtr, newTime, animPtr->currentTime);
			}
			if (currEventFrameidx >= 0 && animPtr->keyFrameEvents[currEventFrameidx].command) {
				animPtr->keyFrameEvents[currEventFrameidx].command->Execute();
			}
		}

		animPtr->currentTime = newTime;

		if (animPtr->currentTime > animPtr->duration)
		{
			animPtr->currentTime = animPtr->duration;

			if (animPtr->repeat == true)
			{
				animPtr->currentTime = 0;
			}
			else
			{
				animPtr = animSequencePtr->NextAnimation();
			}
		}
		if (animPtr->currentTime < 0.f && dt * animPtr->speed < 0.f)
		{
			animPtr->currentTime = 0.f;
			if (animPtr->repeat == true)
			{
				animPtr->currentTime = animPtr->duration;
			}
			else
			{
				animPtr = animSequencePtr->PreviousAnimation();
				animPtr->currentTime = animPtr->duration;
			}
		}

		// Find active keyframes
		if (animPtr->keyFramePositions.size() == 0
			|| animPtr->keyFrameScales.size() == 0
			|| animPtr->keyFrameRotations.size() == 0)
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
		//KeyFrame keyframe = FindCurrentKeyFrame(animPtr, animPtr->currentTime);
		//positionPtr->value = keyframe.position;

		// If there is only 1 KeyFrame in the animation, only use that
		if (animPtr->keyFramePositions.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->position = animPtr->keyFramePositions[0].position;
			return;
		}

		int currPosFrameidx = FindKeyFramePositionIndex(animPtr, animPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currPosFrameidx == animPtr->keyFramePositions.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->position = animPtr->keyFramePositions[currPosFrameidx].position;
			return;
		}

		/* UPDATE POSITIONS */

		// Find a position between the current KeyFrame, and the next KeyFrame
		int nextPosFrameidx = currPosFrameidx + 1;

		const KeyFramePosition& keyFramePos1 = animPtr->keyFramePositions[currPosFrameidx];
		const KeyFramePosition& keyFramePos2 = animPtr->keyFramePositions[nextPosFrameidx];

		// Look into glm's easing functions
		// 1. Ease-In
		// 2. Ease-Out
		// 3. Ease-InOut
		// glm::sineEaseIn()


		// As the animation time 
		// How would I change this to implement glm::sineEaseIn(x)?
		float posFraction = (animPtr->currentTime - keyFramePos1.time) / (keyFramePos2.time - keyFramePos1.time);

		switch (keyFramePos2.easingType)
		{
		case EasingType::EaseIn:
			posFraction = glm::sineEaseIn(posFraction);
			break;
		case EasingType::EaseOut:
			posFraction = glm::sineEaseInOut(posFraction);
			break;
		case EasingType::EaseInOut:
			posFraction = glm::sineEaseInOut(posFraction);
			break;
		default:
			break;
		}
		currentEntityPtr->position = keyFramePos1.position + (keyFramePos2.position - keyFramePos1.position) * posFraction;

		//printf("KeyFrame(%d -> %.2f -> %d) position: (%.2f, %.2f)\n",
		//	CurrentKeyFrameIndex, fraction, NextKeyFrameIndex, positionPtr->value.x, positionPtr->value.y);

		//positionPtr->value = (coords1 + coords2) / 2.f * animPtr->currentTime;

		/* UPDATE SCALES*/

		if (animPtr->keyFrameScales.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->scale = animPtr->keyFrameScales[0].scale;
			return;
		}

		int currScaleFrameidx = FindKeyFrameScaleIndex(animPtr, animPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currScaleFrameidx == animPtr->keyFrameScales.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->scale = animPtr->keyFrameScales[currScaleFrameidx].scale;
			return;
		}

		int nextScaleFrameidx = currScaleFrameidx + 1;

		const KeyFrameScale& keyFrameScale1 = animPtr->keyFrameScales[currScaleFrameidx];
		const KeyFrameScale& keyFrameScale2 = animPtr->keyFrameScales[nextScaleFrameidx];

		float scaleFraction = (animPtr->currentTime - keyFrameScale1.time) / (keyFrameScale2.time - keyFrameScale1.time);

		switch (keyFrameScale2.easingType)
		{
		case EasingType::EaseIn:
			scaleFraction = glm::sineEaseIn(scaleFraction);
			break;
		case EasingType::EaseOut:
			scaleFraction = glm::sineEaseInOut(scaleFraction);
			break;
		case EasingType::EaseInOut:
			scaleFraction = glm::sineEaseInOut(scaleFraction);
			break;
		default:
			break;
		}

		currentEntityPtr->scale = keyFrameScale1.scale + (keyFrameScale2.scale - keyFrameScale1.scale) * scaleFraction;


		/* UPDATE ROTATIONS*/

		if (animPtr->keyFrameRotations.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			currentEntityPtr->rotation = animPtr->keyFrameRotations[0].rotation;
			return;
		}

		int currRotFrameidx = FindKeyFrameRotIndex(animPtr, animPtr->currentTime);

		// If we are at the last KeyFrame, only use that KeyFrame.
		if (currRotFrameidx == animPtr->keyFrameRotations.size() - 1)
		{
			// We only need to set the position of the last KeyFrame.
			currentEntityPtr->rotation = animPtr->keyFrameRotations[currRotFrameidx].rotation;
			return;
		}

		int nextRotFrameidx = currRotFrameidx + 1;

		const KeyFrameRotation& keyFrameRot1 = animPtr->keyFrameRotations[currRotFrameidx];
		const KeyFrameRotation& keyFrameRot2 = animPtr->keyFrameRotations[nextRotFrameidx];

		float rotationFraction = (animPtr->currentTime - keyFrameRot1.time) / (keyFrameRot2.time - keyFrameRot1.time);

		switch (keyFrameRot2.easingType)
		{
		case EasingType::EaseIn:
			rotationFraction = glm::sineEaseIn(rotationFraction);
			break;
		case EasingType::EaseOut:
			rotationFraction = glm::sineEaseInOut(rotationFraction);
			break;
		case EasingType::EaseInOut:
			rotationFraction = glm::sineEaseInOut(rotationFraction);
			break;
		default:
			break;
		}

		//if (keyFrameRot2.InterpolationType == 1)
		//{
		//	currentEntityPtr->rotation = glm::lerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction);
		//}
		//if (keyFrameRot2.InterpolationType == 2)
		//{
		//	currentEntityPtr->rotation = glm::slerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction);
		//}
		//else
		//{
		//	currentEntityPtr->rotation = keyFrameRot1.rotation + (keyFrameRot2.rotation - keyFrameRot1.rotation) * rotationFraction;
		//}
		currentEntityPtr->rotation = glm::slerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction);
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

void AnimationSystem::ColourCodeEasingType(cEntity* currentEntityPtr, EasingType type) {
	if (!currentEntityPtr->mesh) {
		//No mesh, so nothing to colour.
		return;
	}

	switch (type)
	{
	case EasingType::EaseIn:
		// Red
		currentEntityPtr->mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 0.f, 0.f);
		currentEntityPtr->mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
		currentEntityPtr->mesh->objectDebugColourRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
		break;
	case EasingType::EaseOut:
		// Yellow
		currentEntityPtr->mesh->wholeObjectSpecularRGB = glm::vec3(1.f, 1.f, 0.f);
		currentEntityPtr->mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
		currentEntityPtr->mesh->objectDebugColourRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
		break;
	case EasingType::EaseInOut:
		// Green
		currentEntityPtr->mesh->wholeObjectSpecularRGB = glm::vec3(0.f, 1.f, 0.f);
		currentEntityPtr->mesh->wholeObjectDiffuseRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
		currentEntityPtr->mesh->objectDebugColourRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
		break;
	default:
		// No easing...White.
		//currentEntityPtr->mesh->wholeObjectSpecularRGB = glm::vec3(1.f);
		//currentEntityPtr->mesh->wholeObjectDiffuseRGBA = glm::vec4(1.f);
		//currentEntityPtr->mesh->objectDebugColourRGBA = glm::vec4(1.f);
		break;
	}
}