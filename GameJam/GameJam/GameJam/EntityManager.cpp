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

void EntityManager::InitCharacter(void) {
	player = new cCharacter(glm::vec3(0.f, -3.f, 15.f), glm::vec3(0.f, 0.f, 0.f));
}

void EntityManager::ShutDown(void)
{
	if (player) {
		delete player;
		player = 0;
	}
}

const std::vector<cEntity*>&EntityManager::GetEntities(void)
{
	return entities;
}

const std::vector<cEntity*>& EntityManager::GetUIEntities(void)
{
	return ui_entities;
}

cEntity* EntityManager::CreateEntity(void)
{
	cEntity* newEntity = new cEntity();
	entities.push_back(newEntity);
	return newEntity;
}

cEntity* EntityManager::CreateUIEntity(void)
{
	cEntity* newEntity = new cEntity();
	ui_entities.push_back(newEntity);
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
cParticle* EntityManager::CreateParticle(std::string texture, float _lifeSpan, glm::vec3 _position, float transparency, bool followCameraPosition, glm::vec3 _scale) {
	cParticle* newParticle = new cParticle(texture, _lifeSpan, _position, transparency, followCameraPosition, _scale);
	particles.push_back(newParticle);
	return newParticle;
}

void EntityManager::DeleteParticle(cParticle* particle) {
	//Flag it to delete next loop.
	particlesToDelete.push_back(particle);
}
void EntityManager::CleanupParticles() {
	//Could choose better data structure to optimize. For now take brute force approach.
	for (unsigned int delIndex = 0; delIndex < particlesToDelete.size(); delIndex++) {
		for (unsigned int index = 0; index < particles.size(); index++) {
			if (particles[index] == particlesToDelete[delIndex]) {
				particles.erase(particles.begin() + index);
				break;
			}
		}
		delete particlesToDelete[delIndex];
		particlesToDelete[delIndex] = 0;
	}
	particlesToDelete.clear();
}

cProp* EntityManager::CreateProp(std::string name, std::string texture, glm::vec3 position, eShapeType physicsShape, glm::vec3 scale, glm::vec3 physicsScale, float mass, glm::vec3 orientation, glm::vec3 velocity)
{
	cProp* newProp = new cProp(name, texture, position, physicsShape, scale, physicsScale, mass, orientation, velocity);
	entities.push_back((cEntity*)newProp);
	return newProp;
}

void EntityManager::TimeStep(float deltaTime) {
	player->TimeStep(deltaTime);
	for (cParticle* particle : particles) {
		if (particle != nullptr) {
			particle->TimeStep(deltaTime);
		}
	}
	for (cEntity* entity : entities) {
		if (entity->mesh) {
			entity->TimeStep(deltaTime);
		}
	}
	CleanupParticles();
}

cCharacter* EntityManager::GetPlayer(void) {
	return player;
}