#pragma once

#include "cFirework.h"

class cFireworkDebris : public cFirework {
	public:
		using cFirework::cFirework;
		virtual bool isReadyForStageTwo();
	private:
		float _counter = 0;
};