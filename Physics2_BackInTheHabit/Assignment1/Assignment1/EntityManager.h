#pragma once
#include <vector>

#include "cEntity.h"

class EntityManager {
public:
	EntityManager(void);
	~EntityManager(void);

	void StartUp(void);
	void ShutDown(void);

	void TimeStep(float deltaTime);

	const std::vector<cEntity*>& GetEntities(void);
	cEntity* CreateEntity(void);
private:
	std::vector<cEntity*> entities;
};