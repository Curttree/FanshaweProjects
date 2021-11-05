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
		std::vector<iTank*> tanks;
		cMazeManager* mazeManager;
};