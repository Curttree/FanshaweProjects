#pragma once
#include "iFuse.h"

class cTimedFuse : public iFuse {
	public:
		cTimedFuse(float maxTime);
		virtual bool isReadyForNextStage();
	private:
		float _maxTime;
		float _counter = 0;
};