#pragma once

#include <vector>

#include <extern/glm/vec3.hpp>
#include <extern/glm/gtx/quaternion.hpp>

class cBone
{
public:
	cBone(const glm::vec3& _position, const glm::vec3& _scale, const glm::quat& _rotation);
	cBone();

	virtual ~cBone()
	{}

	const glm::vec3& GetPosition();
	void SetPosition(const glm::vec3& newPosition);

	const glm::vec3& GetScale();
	void SetScale(const glm::vec3& scale);

	const glm::quat& GetRotation();
	void SetRotation(const glm::quat& rotation);

	const glm::mat4& GetModelMatrix();
	void SetModelMatrix(const glm::mat4& modelMatrix);

	cBone* GetParent();
	void SetParent(cBone* parent);

	void AddChild(cBone* bone);

	std::vector<cBone*>& GetChildren() { return children; }

	int index;

private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::mat4 modelMatrix;

	cBone* parent;
	std::vector<cBone*> children;
};