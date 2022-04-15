#pragma once
#include <cEntity.h>
#include <glm/detail/type_vec3.hpp>

// Prop is a specialized entity meant to be generated in large volumes.
// They will (likely) not have their own animations, though they will have physics properties.
class cProp : public cEntity {
public:
	cProp(std::string name, std::string texture, glm::vec3 position, glm::vec3 scale = glm::vec3(1.f));
};