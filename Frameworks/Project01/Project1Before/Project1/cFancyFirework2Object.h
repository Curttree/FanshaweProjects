#pragma once

#include "cFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework2Object : public cFireworkObject {
public:
	cFancyFirework2Object(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();	
	// FROM iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
private:
	iMediator* p_Mediator = 0;
};