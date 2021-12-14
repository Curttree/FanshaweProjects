#pragma once
#include "iMessage.h"
#include "cAlien.h"
#include "cUfo.h"
class cGameStateManager : public iMessage {
public:
	void AssembleAliens();
	void Update(float deltaTime);
	void InitializePlayer();

	// Via iMessage

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	std::vector<cAlien*> aliens;

	cUfo* ufo;
	float ufoSpawnTime = 15.f;
	float currentUfoCycleTime = 0.f;
};