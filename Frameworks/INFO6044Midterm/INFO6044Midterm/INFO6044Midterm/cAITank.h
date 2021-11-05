#pragma once
#include "iTank.h"
#include "cMesh.h"
#include "iMediator.h"
#include "cBullet.h"

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
	glm::vec3 heading = glm::vec3(0.f,1.f,0.f);
	int direction = 0;
	TankState state = TankState::WAITING;
	int health = 100;
	float speed = 5;
	float maxWaitTime = 1.f;
	float waitTime = 0.f;
	iMediator* p_Mediator = 0;
	cBullet* activeBullet = 0;
};