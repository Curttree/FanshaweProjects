#pragma once
#include "iWeapon.h"

class cPlayer;

class cBasicWeapon : public iWeapon {
public:
	cBasicWeapon(cPlayer* _owner);
	virtual void Update(float deltaTime);
	virtual void Fire();
	cPlayer* owner;
protected:
	float coolDownTime = 240.f;
	float coolDownTimer = 0.f;
};