#pragma once
#include "cGameEntity.h"

class cPlayer : public cGameEntity {
public:
	cPlayer(float posX, float posY, float scale);
	virtual ~cPlayer();
	void Update(float deltaTime);
	void Thrust(float percentage);
	void KillThrusters();
	void Fire(float weaponNumber = 0);

	//Overloaded methods
	void SetAngle(float _angle);
protected:
	cPlayer();
	void Dampen();

	float thrustSpeed = 0.075f;
	float dampenSpeed = 0.98f;
	Vec2 initialHeading;
	Vec2 heading;
	Vec2 movement;
};