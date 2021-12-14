#pragma once
#include "iTank.h"
#include "cMesh.h"
#include "iMediator.h"
#include "cBullet.h"
#include "cLaser.h"

class cAITank : public iTank {
public:
	cAITank(cMesh* _model);
	// From iTank
	virtual void TimeStep(float deltaTime);
	virtual bool CheckValidMove(glm::vec3 newPos, glm::vec3 heading);
	virtual void MoveLeftRight_X(float deltaTime, float amount);
	virtual void MoveUpDown_Y(float deltaTime, float amount);
	virtual glm::vec3 GetPosition();
	virtual TankState GetState();
	virtual int GetId();
	virtual glm::vec3 GetHeading();
	virtual void SnapToGrid(glm::vec3 position);
	virtual void StopMoving();
	// From iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
	virtual bool CheckLineOfSight();
protected:
	cMesh* model = 0;
	cMesh* laserBubble = 0;
	glm::vec3 heading = glm::vec3(0.f,1.f,0.f);
	int direction = 0;
	TankState state = TankState::WAITING;

	float health = 100.f;
	float startingHealth = 100.f;
	glm::vec4 startingColour = glm::vec4(0.f);

	float speed = 5;
	float maxWaitTime = 1.f;
	float waitTime = 0.f;
	float coolDown = 1.f;
	float currentCoolDown = 0.f;
	bool coolDownActive = false;

	float maxLaserChargeTime = 1.f;
	float currentLaserChargeTime = 0.f;
	virtual void FireSomething();
	virtual void FireLaser();
	std::vector<cLaser*> activeLasers;
	std::vector<cLaser*> laserCleanup;


	iMediator* p_Mediator = 0;
	cBullet* activeBullet = 0;
};