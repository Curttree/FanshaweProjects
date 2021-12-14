#pragma once
#include "iMediator.h"

class cInvaderMediator : public iMediator {
public:
	//Via iMediator
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
};