#pragma once

#include "iMediator.h"
#include "cFireworkObject.h"
#include "cFireworkBuilder.h"

class cFireworkMediator: public iMediator {
public:
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	virtual void TimeStep(float deltaTime);
	std::vector<cFireworkObject*> fireworkObjs;
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};