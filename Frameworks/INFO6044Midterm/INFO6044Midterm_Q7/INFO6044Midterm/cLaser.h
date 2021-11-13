#pragma once
#include "iMediator.h"
#include <extern/glm/vec3.hpp> // glm::vec3
#include "cMesh.h"

class cLaser : public iMessage {
public:
	cLaser(int _ownerID, glm::vec3 _position, glm::vec4 _colour, float damageModifier = 1.f);
	virtual ~cLaser();
	virtual void TimeStep(float deltaTime);
	virtual int GetId();

	// FROM iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	int ownerID = 0;
	float lifeSpan = 1.5f;
	float aliveTime = 0.f;
	float damage = 10.f;
	cMesh* model;
	bool CheckIfHit();
	iMediator* p_Mediator = 0;
};