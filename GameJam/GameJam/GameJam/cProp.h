#pragma once
#include <cEntity.h>
#include <glm/detail/type_vec3.hpp>
#include <Fanshawe2022Physics/interfaces/shapes.h>
#include <Fanshawe2022Physics/interfaces/iRigidBody.h>

// Prop is a specialized entity meant to be generated in large volumes.
// They will (likely) not have their own animations, though they will have physics properties.
class cProp : public cEntity {
public:
	cProp(std::string name, std::string texture, glm::vec3 position, eShapeType physicsShape, glm::vec3 scale = glm::vec3(1.f), glm::vec3 physicsScale = glm::vec3(1.f), float mass = 1.f, glm::vec3 orientation = glm::vec3(0.f), glm::vec3 velocity = glm::vec3(0.f));
	gdp2022Physics::iRigidBody* rigidBody = 0;
	void TimeStep(float deltaTime);
	void Destroy(float inTime);
	glm::vec3 positionOffset;
private:
	bool dying = false;
	float deathMaxTime = 0.f;
	float deathTimer = 0.f;
	void InitializePhysics(eShapeType shape, float mass = 1.f, glm::vec3 scale = glm::vec3(1.f), glm::vec3 orientation = glm::vec3(0.f), glm::vec3 velocity = glm::vec3(0.f));
};