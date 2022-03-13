#pragma once
#include "iWeapon.h"

class cPlayer;

class cBasicWeapon : public iWeapon {
public:
	cBasicWeapon(cPlayer* _owner);
	void Update(float deltaTime);
	void Fire();
	cPlayer* owner;
private:
	float coolDownTime = 180.f;
	float coolDownTimer = 0.f;
};