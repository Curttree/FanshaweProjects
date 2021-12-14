#pragma once
#include "cEntity.h"

class cBullet : public cEntity {
public:
	cBullet();
	virtual void Update(float deltaTime);
};