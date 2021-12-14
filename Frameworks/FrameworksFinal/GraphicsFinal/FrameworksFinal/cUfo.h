#pragma once
#include "cEntity.h"

class cUfo : public cEntity {
public:
	cUfo();
	virtual void Update(float deltaTime);
	virtual void StartMoving();
private:
	float direction;
};