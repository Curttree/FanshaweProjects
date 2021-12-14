#pragma once
#include "cEntity.h"

class cShieldPixel : public cEntity {
public:
	cShieldPixel(glm::vec3 position);
	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
};