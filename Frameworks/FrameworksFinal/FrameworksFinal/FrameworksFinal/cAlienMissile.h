#pragma once
#include "cEntity.h"
class cAlienMissile : public cEntity {
public:
	cAlienMissile();
	virtual void Update(float deltaTime); 
	virtual void StartMoving(glm::vec3 position);
	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	bool hitShield;
	int currentModel = 0;
	float animTime = 0.25f;
	float timeSinceLastChange = 0.f;
	float inShieldTime = 0.f;
	float hitShieldTime = 0.04f;
	std::string models[4] = { "Invaders/SpaceInvader_Bomb_pose_1.ply", "Invaders/SpaceInvader_Bomb_pose_2.ply","Invaders/SpaceInvader_Bomb_pose_3.ply","Invaders/SpaceInvader_Bomb_pose_2.ply" };
};