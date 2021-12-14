#pragma once
#include "iMediator.h"
#include <extern/glm/vec3.hpp> // glm::vec3
#include "cMesh.h"

class cBullet : public iMessage {
public:
	cBullet(int _ownerID, glm::vec3 _position, glm::vec3 _direction);
	virtual ~cBullet();
	virtual void TimeStep(float deltaTime);

	// FROM iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	int ownerID = 0; 
	float speed = 20.f;
	float damage = 10.f;
	glm::vec3 direction;
	cMesh* model;
	bool CheckValidMove();
	iMediator* p_Mediator = 0;
};