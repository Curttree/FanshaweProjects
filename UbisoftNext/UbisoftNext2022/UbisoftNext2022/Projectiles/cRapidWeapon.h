#pragma once
#include "cBasicWeapon.h"

class cPlayer;

class cRapidWeapon : public cBasicWeapon {
public:
	cRapidWeapon(cPlayer* _owner) : cBasicWeapon::cBasicWeapon(_owner) {};
	virtual void Fire();
protected:
	float coolDownTime = 90.f;
};