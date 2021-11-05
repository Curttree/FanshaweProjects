#pragma once
#include "cWorldSpace.h"

class iFuse {
public:
	virtual bool isReadyForNextStage() = 0;
protected:
	// Noticed partway through my 'After' that I had used a non-method in my interface.
	// Keeping the name as iFuse for now, but will be sure to split this into a separate iFuse and cFuse in future implementations.
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};