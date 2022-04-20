#include "Character.h"

namespace gdp2022Physics
{
	void Character::Initialize() {

	}

	glm::vec3 Character::GetPosition() {
		return glm::vec3(0.f);
	}
	glm::quat Character::GetRotation() {
		return glm::quat(glm::vec3(0.f));
	}
}