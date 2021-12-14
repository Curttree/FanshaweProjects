#pragma once
#include "cEntity.h"
#include "cBullet.h"

class cPlayer : public cEntity {
public:
	cPlayer();
	void MoveLeft();
	void MoveRight();
	void Stop(int direction);
	void Fire();
	virtual void Update(float deltaTime);
	cBullet* bullet;
private:
	bool movingLeft = false;
	bool movingRight = false;
};