#include "cParticleEmitter.h"
#include "cParticleFactory.h"
#include "globalFunctions.h"

cParticleEmitter::cParticleEmitter(unsigned int _particleType, glm::vec3 _position, float _spread, glm::vec3 _direction, float _spawnTime, float _maxScale, float _minScale) :
	particleType(_particleType), position(_position), spread(_spread), direction(_direction), maxSpawnTime(_spawnTime), maxScale(_maxScale), minScale(_minScale)
{

}
cParticleEmitter::~cParticleEmitter() {
	//Any created particles will be disposed of by the entityManager.
}

void cParticleEmitter::TimeStep(float deltaTime) {
	spawnTimer += deltaTime;
	if (spawnTimer >= maxSpawnTime) {
		spawnTimer = 0.f;
		glm::vec3 spawnPosition = position;
		spawnPosition.x += ::gGetRandBetween(-spread, spread);
		spawnPosition.z += ::gGetRandBetween(-spread, spread);
		cParticle* particle = cParticleFactory::Instance()->createParticle(particleType, spawnPosition, false,direction * deltaTime);
		particle->scale *= glm::vec3(::gGetRandBetween(minScale,maxScale));
		particle->mesh->scale = glm::vec3(particle->scale);

	}
}

void cParticleEmitter::SetPosition(glm::vec3 newPos) {
	position = newPos;
}

