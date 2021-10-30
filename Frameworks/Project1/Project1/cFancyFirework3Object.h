#pragma once

#include "cFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework3Object : public cFireworkObject {
public:
	cFancyFirework3Object(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};