#pragma once
#include "cParticle.h"

class cParticleEmitter {
public:
	cParticleEmitter(unsigned int _particleType, glm::vec3 _position, float _spread, glm::vec3 _direction, float _spawnTime, float _maxScale = 1.f, float _minScale = 1.f);
	virtual ~cParticleEmitter();
	void TimeStep(float deltaTime);
private:
	float spawnTimer = 0.f;
	float maxSpawnTime = 0.f;
	float spread = 0.f;
	float maxScale = 1.f;
	float minScale = 1.f;
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 direction = glm::vec3(0.f, 1.f, 0.f);
	unsigned int particleType = 0;
};