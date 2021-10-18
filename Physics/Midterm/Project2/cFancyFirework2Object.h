#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework2Object : public iFireworkObject {
public:
	cFancyFirework2Object();
	cFancyFirework2Object(cFirework* _particle, cModel* _model);
	virtual std::vector<iFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};