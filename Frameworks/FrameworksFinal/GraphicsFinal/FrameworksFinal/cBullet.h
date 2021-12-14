#pragma once
#include "cEntity.h"

class cBullet : public cEntity {
public:
	cBullet();
	virtual void Update(float deltaTime);

	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
};