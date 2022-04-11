#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

bool Compare(const glm::mat4& a, const glm::mat4& b);
glm::mat4 Convert(const aiMatrix4x4& m);
void Convert(const aiMatrix4x4& in, glm::mat4& out);
void Convert(const aiVector3D& in, glm::vec3& out);
void Convert(const aiQuaternion& in, glm::quat& out);
