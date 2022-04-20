#pragma once
#include <extern/glm/fwd.hpp>

class iCharacter {
public:
	virtual void Initialize() = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual glm::quat GetRotation() = 0;
};