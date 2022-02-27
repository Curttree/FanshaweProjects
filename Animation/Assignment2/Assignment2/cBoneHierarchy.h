#pragma once

#include "Component.h"
#include "cBone.h"

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
};

struct cBoneHierarchy : Component
{
	cBone* rootBone;
	std::vector<cBone*> bones;
	std::vector<Transform> currentTransforms;
	float time;
};