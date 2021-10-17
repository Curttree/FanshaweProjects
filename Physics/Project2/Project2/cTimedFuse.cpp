#include "cTimedFuse.h"

cTimedFuse::cTimedFuse(float maxTime) {
	_maxTime = maxTime;
}

bool cTimedFuse::isReadyForStageTwo() {
	if (_counter < _maxTime) {
		_counter++;
		return false;
	}
	return true;
}