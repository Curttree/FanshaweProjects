#pragma once

struct sCannonDef {
	float upperYaw = 0.25f * glm::pi<float>();
	float lowerYaw = -0.25f * glm::pi<float>();
	float upperPitch =(7.f/8.f);
	float lowerPitch = 0.f;
};