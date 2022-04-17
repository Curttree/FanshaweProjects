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

const std::vector<cParticle*>& EntityManager::GetParticles(void) {
	return particles;
}
cParticle* EntityManager::CreateParticle(float lifeSpan) {
	cParticle* newParticle = new cParticle(lifeSpan);
	particles.push_back(newParticle);
	return newParticle;
}

void EntityManager::DeleteParticle(cParticle* particle) {
	//Could choose better data structure to optimize. For now take brute force approach.
	for (unsigned int index = 0; index < particles.size(); index++) {
		if (particles[index] == particle) {
			particles.erase(particles.begin() + index);
		}
	}
	delete particle;
}

cProp* EntityManager::CreateProp(std::string name, std::string texture, glm::vec3 position, eShapeType physicsShape, glm::vec3 scale, glm::vec3 physicsScale, float mass, glm::vec3 orientation, glm::vec3 velocity)
{
	cProp* newProp = new cProp(name, texture, position, physicsShape, scale, physicsScale, mass, orientation, velocity);
	entities.push_back((cEntity*)newProp);
	return newProp;
}

void EntityManager::TimeStep(float deltaTime) {
	for (cEntity* entity : entities) {
		if (entity->mesh) {
			entity->TimeStep(deltaTime);
		}
	}
	for (cParticle* particle : particles) {
		particle->TimeStep(deltaTime);
	}
}