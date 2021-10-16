#pragma once

#include "iFireworkObject.h"

class cFireworkDebrisObject : public iFireworkObject {
	using iFireworkObject::iFireworkObject;
	virtual std::vector<iFireworkObject*> triggerStageTwo();
	virtual bool isReadyForStageTwo();
private:
	float _counter = 0;
};