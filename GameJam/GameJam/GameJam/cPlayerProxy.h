#pragma once
#include <cEntity.h>
#include <Fanshawe2022Physics/interfaces/iRigidBody.h>
#include <Fanshawe2022Physics/interfaces/shapes.h>

class cPlayerProxy : public cEntity {
public:
	cPlayerProxy(glm::vec3 position);
	void TimeStep(float deltaTime);
	void ResetOrientation();
	gdp2022Physics::iRigidBody* rigidBody = 0;
	glm::vec3 velocity = glm::vec3(0.f);	// Could make private and add getter.
private:
	void InitializePhysics(eShapeType shape, float mass = 1.f, glm::vec3 scale = glm::vec3(1.f));
	float walkSpeed;
	float runSpeed;
};