#include "BoneSystem.h"

#include "cBoneHierarchy.h"

void BoneSystem::Process(const std::vector<cEntity*>& entities, float dt)
{
	cBoneHierarchy* boneHierarchy;

	for (int i = 0; i < entities.size(); ++i)
	{
		boneHierarchy = entities[i]->mesh->bones;

		if (boneHierarchy == 0)
			continue;

		boneHierarchy->time += dt;
		UpdateHierarchy(boneHierarchy);
	}
}

void BoneSystem::PlaySequence(const std::vector<cEntity*>& entities, int sequenceNumber) {
	//Animation sequences could be moved to the entity depending on how we are going to structure things moving forward.
	currentSequence = sequenceNumber;
	cBoneHierarchy* boneHierarchy;
	for (int i = 0; i < entities.size(); ++i)
	{
		boneHierarchy = entities[i]->mesh->bones;

		if (boneHierarchy == 0)
			continue;

		boneHierarchy->time = 0.f;
		UpdateHierarchy(boneHierarchy);
	}
}


void BoneSystem::UpdateHierarchy(cBoneHierarchy* boneHierarchy)
{

	glm::quat start = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::quat end = glm::quat(.7071f, 0.f, 0.f, 0.7071f);

	float rotationFraction = boneHierarchy->time / 5.f;
	glm::quat currentRotation = glm::slerp(start, end, rotationFraction);

	UpdateBone(boneHierarchy, boneHierarchy->rootBone, glm::mat4(1.f));
}

void BoneSystem::UpdateBone(cBoneHierarchy* boneHierarchy, cBone* bone,
	const glm::mat4 parentModelMatrix)
{
	Transform& transform = boneHierarchy->currentTransforms[bone->index];

	glm::mat4 modelMatrix = parentModelMatrix * transform.value;
	bone->SetModelMatrix(modelMatrix);

	std::vector<cBone*>& children = bone->GetChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		UpdateBone(boneHierarchy, children[i], modelMatrix);
	}
}
