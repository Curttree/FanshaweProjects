#pragma once

#include "iFireworkObject.h"
#include "cFireworkBuilder.h"

class cFancyFirework4Object : public iFireworkObject {
public:
	cFancyFirework4Object();
	cFancyFirework4Object(cFirework* _particle, cModel* _model);
	virtual std::vector<iFireworkObject*> triggerNextStage();
protected:
	cFireworkBuilder* fireworkBuilder = cFireworkBuilder::Instance();
};
