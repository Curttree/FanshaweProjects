#pragma once
#include "cBullet.h"

class cSuperBullet : public cBullet {
public:
	cSuperBullet(int _ownerID, glm::vec3 _position, glm::vec3 _direction, float damageModifier = 1.f);
	void Redirect();
protected:
	bool redirected = false; 
	virtual bool CheckValidMove();
};