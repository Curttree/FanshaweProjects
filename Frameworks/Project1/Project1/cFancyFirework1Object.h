#pragma once

#include "cFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework1Object : public cFireworkObject {
public:
	cFancyFirework1Object(cFirework* _particle, cModel* _model);
	virtual std::vector<cFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};