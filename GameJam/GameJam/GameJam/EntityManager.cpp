#include "EntityManager.h"

EntityManager::EntityManager(void)
{
}

EntityManager::~EntityManager(void)
{
}

void EntityManager::StartUp(void)
{

}

void EntityManager::ShutDown(void)
{
}

const std::vector<cEntity*>&EntityManager::GetEntities(void)
{
	return entities;
}

cEntity* EntityManager::CreateEntity(void)
{
	cEntity* newEntity = new cEntity();
	entities.push_back(newEntity);
	return newEntity;
}

void EntityManager::DeleteEntity(cEntity* entity)
{
	//Could choose better data structure to optimize. For now take brute force approach.
	for (unsigned int index = 0; index < entities.size(); index++) {
		if (entities[index] == entity) {
			entities.erase(entities.begin() + index);
		}
	}
	delete entity;
}


cProp* EntityManager::CreateProp(std::string name, std::string texture, glm::vec3 position, glm::vec3 scale)
{
	cProp* newProp = new cProp(name, texture, position, scale);
	entities.push_back((cEntity*)newProp);
	return newProp;
}

void EntityManager::TimeStep(float deltaTime) {
	for (cEntity* entity : entities) {
		if (entity->mesh) {
			entity->TimeStep(deltaTime);
		}
	}
}