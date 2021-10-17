#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework1Object : public iFireworkObject {
public:
	using iFireworkObject::iFireworkObject;
	virtual std::vector<iFireworkObject*> triggerStageTwo();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};