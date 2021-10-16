#pragma once

#include "cFireworkObject.h"

class cFireworkDebrisObject : public cFireworkObject {
	using cFireworkObject::cFireworkObject;
	virtual std::vector<cFireworkObject*> triggerStageTwo();
};