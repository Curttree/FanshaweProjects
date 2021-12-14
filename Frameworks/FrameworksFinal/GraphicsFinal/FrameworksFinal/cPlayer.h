#pragma once
#include "cEntity.h"

class cPlayer : public cEntity {
public:
	cPlayer();
	void MoveLeft();
	void MoveRight();
	void Stop();
	virtual void Update(float deltaTime);
};