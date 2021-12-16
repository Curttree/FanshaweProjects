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
	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	bool movingLeft = false;
	bool movingRight = false;
};