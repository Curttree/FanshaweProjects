#pragma once

#include "iFireworkObject.h"
#include "cFireworkFactory.h"

class cFancyFirework2Object : public iFireworkObject {
	using iFireworkObject::iFireworkObject;
	virtual std::vector<iFireworkObject*> triggerStageTwo();
	virtual bool isReadyForStageTwo();
protected:
	cFireworkFactory* fireworkFactory = cFireworkFactory::Instance();
};