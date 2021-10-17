#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework1Object : public iFireworkObject {
public:
	cFancyFirework1Object();
	cFancyFirework1Object(cFirework* _particle, cModel* _model);
	virtual std::vector<iFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};