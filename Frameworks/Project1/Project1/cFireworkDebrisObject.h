#pragma once

#include "cFireworkObject.h"

class cFireworkDebrisObject : public cFireworkObject {
public:
	cFireworkDebrisObject(cFirework* _particle, cModel* _model, glm::vec3 _colour);
	cFireworkDebrisObject(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();
	// FROM iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
private:
	float _counter = 0;
	iMediator* p_Mediator = 0;
};