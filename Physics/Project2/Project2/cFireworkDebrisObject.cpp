#include "cFireworkDebrisObject.h"

std::vector<iFireworkObject*> cFireworkDebrisObject::triggerStageTwo() {
	return std::vector<iFireworkObject*>();
}
bool cFireworkDebrisObject::isReadyForStageTwo() {
	if (_counter < 120.f) {
		_counter++;
		return false;
	}
	return true;
}