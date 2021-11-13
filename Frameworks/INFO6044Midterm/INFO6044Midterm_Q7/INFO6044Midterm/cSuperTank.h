#pragma once
#include "cAITank.h"
#include "cSuperBullet.h"

class cSuperTank : public cAITank {
public:
	cSuperTank(cMesh* _model);
	virtual void TimeStep(float deltaTime);
	virtual bool RecieveMessage(sMessage theMessage);
protected:
	virtual void FireSomething();
	cSuperBullet* activeSuperBullet = 0;
};