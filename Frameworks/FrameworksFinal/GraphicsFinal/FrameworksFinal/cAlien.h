#pragma once
#include "cEntity.h"

class cAlien : public cEntity {
public:
	cAlien(int type);
	cAlien(std::string _pose1, std::string _pose2);
	virtual ~cAlien();
	virtual void Update(float deltaTime);
	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	std::string pose1;
	std::string pose2;
	bool isFirstPose = true;
	float deathTimer = 1.f;
	float currentDeathTime = 0.f;
	bool isDead = false;
};