#include "cParticleObject.h"

cParticleObject::cParticleObject() {
}

cParticleObject::cParticleObject(nPhysics::cParticle* _particle, cModel* _model) {
	particle = _particle;
	model = _model;
}

cParticleObject::cParticleObject(sParticleTemplate* _particleTemp, cModel* _model, glm::vec3 direction, glm::vec3 position) {
	particle = new nPhysics::cParticle(_particleTemp->mass,position);
	particle->SetVelocity(_particleTemp->muzzleVelocity * direction);
	particle->SetDamping(_particleTemp->damping);
	if (_particleTemp->accelerate) {
		particle->SetAcceleration(direction * 2.f);		// Playing up acceleration so it is more obvious.
	}
	model = _model;
	model->scale = _particleTemp->size;
	model->bOverriveVertexColourHACK = true;
	model->vertexColourOverrideHACK = _particleTemp->colour;
	distanceLimit = _particleTemp->distanceLimit;
	timeLimit = _particleTemp->timeLimit;
	age = 0.f;
	origin = position;
	worldSpace->_world->AddParticle(particle);
	if (_particleTemp->applyGravity) {
		worldSpace->_world->GetForceRegistry()->Register(particle, worldSpace->_gravityGenerator);
	}
}

// Always return false if distance limit is negative.
bool cParticleObject::isAtDistanceLimit() {
	if (distanceLimit > 0) {
		float testVal = glm::length(particle->GetPosition() - origin);
		return (testVal >= distanceLimit);
	}
	return false;
}

// Always return false if time limit is negative.
bool cParticleObject::isAtTimeLimit() {
	if (timeLimit > 0) {
		if (age >= timeLimit) {
			return true;
		}
		age++;
		return false;
	}
	return false;
}

bool cParticleObject::isAboveGround() {
	return particle->GetPosition().y >= 0;
}