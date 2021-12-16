#pragma once
#include "cEntity.h"
class cPuck : public cEntity {
public:
	cPuck(glm::vec3 position, glm::vec3 velocity = glm::vec3(0.f));
};