#pragma once

#include "iMessage.h"

class iMediator : public iMessage {
public:
	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse) = 0;
	virtual bool SetReciever(iMediator* pTheReciever) = 0;
};