#pragma once
#include <vector>
#include <Fanshawe2022Physics/interfaces/eShapeType.h>

#include "cEntity.h"
#include "cProp.h"
#include "cParticle.h"

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

	const std::vector<cParticle*>& GetParticles(void);
	cParticle* CreateParticle(std::string texture, float _lifeSpan, glm::vec3 _position, bool followCameraPosition, glm::vec3 _scale);
	void DeleteParticle(cParticle* particle);

	cProp* CreateProp(std::string name, std::string texture, glm::vec3 position, eShapeType physicsShape, glm::vec3 scale = glm::vec3(1.f), glm::vec3 physicsScale = glm::vec3(1.f), float mass = 1.f, glm::vec3 orientation = glm::vec3(0.f), glm::vec3 velocity = glm::vec3(0.f));
private:
	std::vector<cEntity*> entities;
	std::vector<cParticle*> particles;
};