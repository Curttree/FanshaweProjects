#pragma once
#include <extern/glm/glm.hpp>
#include <extern/glm/vec3.hpp> // glm::vec3

class iAnimal {
public:
	iAnimal() {};
	virtual ~iAnimal() {};
	virtual void TimeStep(float deltaTime) = 0;
	virtual void Born(glm::vec3 position) = 0;
	virtual void LocateFood() = 0;
	virtual void EatFood() = 0;
};