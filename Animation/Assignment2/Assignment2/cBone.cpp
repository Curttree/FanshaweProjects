#include "cBone.h"

cBone::cBone(const glm::vec3& _position, const glm::vec3& _scale, const glm::quat& _rotation)
	: position(_position)
	, scale(_scale)
	, rotation(_rotation)
	, parent(0)
{}

cBone::cBone()
	: cBone(glm::vec3(0.f)
		, glm::vec3(1.f)
		, glm::quat(1.f, 0.f, 0.f, 0.f))
{}

const glm::vec3& cBone::GetPosition() { return position; }
void cBone::SetPosition(const glm::vec3& newPosition) { position = newPosition; }

const glm::vec3& cBone::GetScale() { return scale; }
void cBone::SetScale(const glm::vec3& newScale) { scale = newScale; }

const glm::quat& cBone::GetRotation() { return rotation; }
void cBone::SetRotation(const glm::quat& newRotation) { rotation = newRotation; }

const glm::mat4& cBone::GetModelMatrix() { return modelMatrix; }
void cBone::SetModelMatrix(const glm::mat4& newModelMatrix) { modelMatrix = newModelMatrix; }

cBone* cBone::GetParent() { return parent; }
void cBone::SetParent(cBone* parent)
{
	// TODO: Make sure the parent is not a child
	parent = parent;
}

void cBone::AddChild(cBone* bone)
{
	// TODO: Need to check if the bone is already a child
	// TODO: Need to check if the bone is the parent
	children.push_back(bone);
}