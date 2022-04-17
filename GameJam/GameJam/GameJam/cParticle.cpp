#include "cParticle.h"
#include "globals.h"

cParticle::cParticle(float _lifeSpan) : lifeSpan(_lifeSpan){
}

void cParticle::TimeStep(float deltaTime) {
	aliveTimer += deltaTime;
	if (aliveTimer > lifeSpan) {
		::g_pGameEngine->entityManager.DeleteParticle(this);
	}
}