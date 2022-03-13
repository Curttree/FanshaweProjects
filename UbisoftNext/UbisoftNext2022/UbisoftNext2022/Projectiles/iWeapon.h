#pragma once

class iWeapon {
public:
	iWeapon() {};
	virtual ~iWeapon() {};
	virtual void Fire() = 0;
	virtual void Update(float deltaTime) = 0;
};