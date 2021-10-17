#pragma once
#include "cWorldSpace.h"

class iFuse {
public:
	virtual bool isReadyForStageTwo() = 0;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};