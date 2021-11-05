#pragma once
#include "iMediator.h"

#include "iTank.h"
#include "cMazeManager.h"

class cCombatMediator : public iMediator {
	public:
		cCombatMediator();
		virtual ~cCombatMediator();
		virtual bool RecieveMessage(sMessage theMessage);
		virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
		virtual bool SetReciever(iMediator* pTheReciever);
	private:
		virtual void InitializeTanks();
		virtual void TimeStep(float deltaTime);
		virtual void ValidateSight(iTank* observer, sMessage& response);
		virtual void ValidateBulletPosition(int ownerID, glm::vec3 position);
		virtual void CleanupBullet(int ownerID);
		std::vector<iTank*> tanks;
		cMazeManager* mazeManager;
};