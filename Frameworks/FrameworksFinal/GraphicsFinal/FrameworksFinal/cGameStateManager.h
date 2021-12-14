#pragma once
#include "iMessage.h"
#include "cAlien.h"
class cGameStateManager : public iMessage {
public:
	void AssembleAliens();
	void Update(float deltaTime);

	// Via iMessage

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	std::vector<cAlien*> aliens;
};