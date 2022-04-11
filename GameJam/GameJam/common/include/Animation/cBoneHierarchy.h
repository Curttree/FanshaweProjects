#pragma once

#include "Component.h"
#include "cBone.h"

struct Transform
{
	Transform(const glm::mat4& value) : value(value) { }
	Transform() : value(glm::mat4(0.f)) { }
	glm::mat4 value;
};

struct cBoneHierarchy : Component
{
	cBone* rootBone;
	std::vector<cBone*> bones;
	std::vector<Transform> currentTransforms;
	float time;
};