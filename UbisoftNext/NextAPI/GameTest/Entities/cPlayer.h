#pragma once
#include "cGameEntity.h"
#include "..\Projectiles\iWeapon.h"

class cPlayer : public cGameEntity {
public:
	cPlayer(float posX, float posY, float scale);
	virtual ~cPlayer();
	void Update(float deltaTime);
	void Thrust(float percentage);
	void Pull(Vec2 direction, float magnitude);
	void Fire(float weaponNumber = 0);

	Vec2 GetHeading();

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

	iWeapon* primaryWeapon = 0;
};