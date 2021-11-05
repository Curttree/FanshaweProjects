#pragma once
#include "iFuse.h"

class cManualFuse : public iFuse {
public:
	cManualFuse();
	virtual bool isReadyForNextStage();
};