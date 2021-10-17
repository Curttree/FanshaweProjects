#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework3Object : public iFireworkObject {
public:
	cFancyFirework3Object();
	cFancyFirework3Object(cFirework* _particle, cModel* _model);
	virtual std::vector<iFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};