#pragma once

#include "System.h"
#include <extern/glm/vec3.hpp>
#include <extern/glm/gtx/quaternion.hpp>
#include <extern/glm/mat4x4.hpp>

struct cBoneHierarchy;
class cBone;

class BoneSystem : System
{
public:
	virtual void Process(const std::vector<cEntity*>& entities, float dt);
	virtual void PlaySequence(const std::vector<cEntity*>& entities, int sequenceNumber);
private:
	void UpdateHierarchy(cBoneHierarchy* boneHierarchy);
	void UpdateBone(cBoneHierarchy* boneHierarchy, cBone* bone,
		const glm::mat4 parentModelMatrix);
	float animationTime = 10.f;
	int currentSequence;
	void SequenceOne(cBoneHierarchy* boneHierarchy);
	void SequenceTwo(cBoneHierarchy* boneHierarchy);
	void SequenceThree(cBoneHierarchy* boneHierarchy);
	void SequenceFour(cBoneHierarchy* boneHierarchy);
	void SequenceFive(cBoneHierarchy* boneHierarchy);
};