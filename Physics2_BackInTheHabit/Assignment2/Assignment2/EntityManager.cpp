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

void EntityManager::TimeStep(float deltaTime) {
	for (cEntity* entity : entities) {
		if (entity->mesh && entity->rigidBody) {
			entity->mesh->positionXYZ = entity->rigidBody->GetPosition();
			entity->mesh->orientationXYZ = glm::eulerAngles(entity->rigidBody->GetOrientation());
		}
	}
}