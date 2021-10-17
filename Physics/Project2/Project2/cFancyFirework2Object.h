#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework2Object : public iFireworkObject {
	using iFireworkObject::iFireworkObject;
	virtual std::vector<iFireworkObject*> triggerStageTwo();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};