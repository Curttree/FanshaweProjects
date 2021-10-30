#pragma once

#include "cFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework2Object : public cFireworkObject {
public:
	cFancyFirework2Object(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};