#include "cFireworkDebris.h"

bool cFireworkDebris::isReadyForStageTwo() {
	if (_counter < 120.f) {
		_counter++;
		return false;
	}
	return true;
}
