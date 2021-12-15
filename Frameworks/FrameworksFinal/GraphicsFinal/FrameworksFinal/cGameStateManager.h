#pragma once
#include "iMessage.h"
#include "cAlien.h"
#include "cUfo.h"
#include "cShieldPixel.h"
#include "cAlienMissile.h"
class cGameStateManager : public iMessage {
public:
	void AssembleAliens();
	void BuildShields();
	void Update(float deltaTime);
	void InitializePlayer(); 
	virtual bool GameStarted();
	virtual void StartGame();
	virtual bool GameEnded();
	// Via iMessage

	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	std::vector<cAlien*> aliens;
	std::vector<cAlien*> livingAliens;
	std::vector<cShieldPixel*> shields;
	bool gameHasStarted = false;
	bool playerIsAlive = true;
	bool gameHasEnded = false;

	void BuildAShield(float x_offset);
	cUfo* ufo;
	float ufoSpawnTime = 15.f;
	float currentUfoCycleTime = 0.f;

	bool hasMovedDown = false;

	cAlienMissile* missile;
	float missileSpawnTime = 3.f;
	float currentMissileCycleTime = 0.f;

	float gameOverCounter = 0.f;
	float gameOverTime = 3.f;
};