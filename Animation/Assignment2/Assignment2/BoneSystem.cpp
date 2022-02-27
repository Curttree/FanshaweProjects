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

void BoneSystem::UpdateHierarchy(cBoneHierarchy* boneHierarchy /*, Animation* animation */)
{

	glm::vec3 posStart = glm::vec3(0.f,0.f,0.f);
	glm::vec3 posMid = glm::vec3(0.f,3.f,2.f);
	glm::vec3 posEnd = glm::vec3(0.f, 0.f, 0.f);

	glm::vec3 scaleStart = glm::vec3(1.f);
	glm::vec3 scaleMid = glm::vec3(5.f,1.f,1.f);
	glm::vec3 scaleEnd =glm::vec3(1.f);

	glm::quat rotationStart = glm::quat(glm::vec3(0.f,0.f,0.f));
	glm::quat rotationEnd = glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f));
	if (boneHierarchy->time >= animationTime) {
		boneHierarchy->time = 0.f;
	}
	float animationFraction = boneHierarchy->time / animationTime;
	glm::vec3 currentScale = scaleStart;
	glm::vec3 currentPos = scaleStart;
	if (animationFraction < 0.5f) {
		currentScale = scaleStart + (scaleMid - scaleStart) * animationFraction * 2.f;
		currentPos = posStart + (posMid - posStart) * animationFraction * 2.f;
	}
	else {
		currentScale = scaleMid + (scaleEnd - scaleMid) * (animationFraction - 0.5f) * 2.f;
		currentPos = posMid + (posEnd - posMid) * (animationFraction - 0.5f) * 2.f;
	}
	glm::quat currentRotation = glm::slerp(rotationStart, rotationEnd, animationFraction);

	for (int i = 0; i < boneHierarchy->currentTransforms.size(); ++i)
	{
		boneHierarchy->currentTransforms[i].position = currentPos;
		boneHierarchy->currentTransforms[i].rotation = currentRotation;
		boneHierarchy->currentTransforms[i].scale = currentScale;
	}

	UpdateBone(boneHierarchy, boneHierarchy->rootBone, glm::mat4(1.f));
}

void BoneSystem::UpdateBone(cBoneHierarchy* boneHierarchy, cBone* bone,
	const glm::mat4 parentModelMatrix)
{
	Transform& transform = boneHierarchy->currentTransforms[bone->index];

	bone->SetPosition(transform.position);
	bone->SetScale(transform.scale);
	bone->SetRotation(transform.rotation);

	glm::mat4 TranslationMatrix = glm::translate(parentModelMatrix, transform.position);
	glm::mat4 RotationMatrix = glm::mat4_cast(transform.rotation);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	bone->SetModelMatrix(ModelMatrix);

	std::vector<cBone*>& children = bone->GetChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		UpdateBone(boneHierarchy, children[i], ModelMatrix);
	}
}