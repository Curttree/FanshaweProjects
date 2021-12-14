#pragma once
#include "cEntity.h"

class cUfo : public cEntity {
public:
	cUfo();
	virtual void Update(float deltaTime);
	virtual void StartMoving();
	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	float direction;
};