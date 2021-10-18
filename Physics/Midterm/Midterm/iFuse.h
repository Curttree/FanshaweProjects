#pragma once
#include "cWorldSpace.h"

class iFuse {
public:
	virtual bool isReadyForNextStage() = 0;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};