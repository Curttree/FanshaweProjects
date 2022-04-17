#pragma once
#include <cEntity.h>
class cParticle : public cEntity {
public:
	cParticle(float _lifeSpan);
	virtual void TimeStep(float deltaTime);
private:
	float aliveTimer = 0.f;
	float lifeSpan = 5.f;
};