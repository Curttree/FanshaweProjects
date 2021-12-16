#pragma once
#include <extern/glm/vec3.hpp> // glm::vec3
#include <extern/glm/ext/scalar_constants.hpp>

struct sShotDef {
	float upperYaw = 0.25f * glm::pi<float>();
	float lowerYaw = -0.25f * glm::pi<float>();
	float upperPitch = (7.f / 8.f);
	float lowerPitch = 0.f;
	float velocity = 0.f;
	glm::vec3 direction;
	int currentLocation = 0;
	float playerTransparency = 1.f;
	glm::vec3 playerOffset = glm::vec3(0.f);
};