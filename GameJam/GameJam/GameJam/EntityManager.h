#pragma once
#include <vector>

#include "cEntity.h"
#include "cProp.h"

class EntityManager {
public:
	EntityManager(void);
	~EntityManager(void);

	void StartUp(void);
	void ShutDown(void);

	bool GetCollision(glm::vec3 initialPosition, glm::vec3 heading, float magnitude, glm::vec3& location);

	void TimeStep(float deltaTime);

	const std::vector<cEntity*>& GetEntities(void);
	cEntity* CreateEntity(void);

	void DeleteEntity(cEntity* entity);
	cProp* CreateProp(std::string name, std::string texture, glm::vec3 position, glm::vec3 scale = glm::vec3(1.f));
private:
	std::vector<cEntity*> entities;
};