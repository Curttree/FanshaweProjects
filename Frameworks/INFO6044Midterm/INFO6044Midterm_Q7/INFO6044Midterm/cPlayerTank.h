#pragma once
#include "iTank.h"
#include "cMesh.h"
#include "cBullet.h"
#include "cLaser.h"
#include "iMediator.h"

class cPlayerTank : public iTank {
public:
	cPlayerTank(cMesh* _model);
	//From iTank
	virtual void TimeStep(float deltaTime);
	virtual bool CheckValidMove(glm::vec3 newPos, glm::vec3 heading);
	virtual void MoveLeftRight_X(float deltaTime, float amount);
	virtual void MoveUpDown_Y(float deltaTime, float amount);
	virtual glm::vec3 GetPosition();
	virtual TankState GetState();
	virtual int GetId();
	virtual glm::vec3 GetHeading();
	virtual cMesh* GetMesh();
	virtual void SnapToGrid(glm::vec3 position);
	virtual void StopMoving();
	//From iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
protected:
	cMesh* model = 0;
	cMesh* laserBubble = 0;
	glm::vec3 heading = glm::vec3(0.f, 1.f, 0.f);
	TankState state = TankState::WAITING;

	float health = 100.f;
	float startingHealth = 100.f;
	glm::vec4 startingColour = glm::vec4(0.f);

	float coolDown = 0.5f;
	float currentCoolDown = 0.f;

	float maxLaserChargeTime = 1.f;
	float currentLaserChargeTime = 0.f;
	virtual void FireLaser();

	bool coolDownActive = false;
	cBullet* activeBullet = 0;
	std::vector<cLaser*> activeLasers;
	std::vector<cLaser*> laserCleanup;

	float speed = 10; 
private:
	iMediator* p_Mediator = 0;
};