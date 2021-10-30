#pragma once

#include "iMessage.h"

class cFireworkMediator: public iMessage {

	virtual void RecieveMessage(sMessage theMessage);
	virtual void SetReciever(iMessage* pTheReciever);
};