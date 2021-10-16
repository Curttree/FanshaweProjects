#pragma once

#include "cFireworkObject.h"

class cFancyFirework1Object : public cFireworkObject {
	virtual std::vector<cFireworkObject*> triggerStageTwo();
};