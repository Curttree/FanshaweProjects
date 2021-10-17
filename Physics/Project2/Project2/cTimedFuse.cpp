#include "cTimedFuse.h"

cTimedFuse::cTimedFuse(float maxTime) {
	_maxTime = maxTime;
}

bool cTimedFuse::isReadyForNextStage() {
	if (_counter < _maxTime) {
		_counter++;
		return false;
	}
	return true;
}