#include "cManualFuse.h"

cManualFuse::cManualFuse() {
}

bool cManualFuse::isReadyForNextStage() {
	// A manual fuse will never be triggered independently. It will rely on the firework
	// receiving a signal from the mediator to explode.
	return false;
}