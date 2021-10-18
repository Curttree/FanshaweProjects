#include "cApexFuse.h"

cApexFuse::cApexFuse(bool* _hitApex) {
	hitApex = _hitApex;
}

cApexFuse::~cApexFuse() {
	if (hitApex) {
		delete hitApex;
		hitApex = NULL;
	}
}

bool cApexFuse::isReadyForNextStage() {
	return *hitApex;
}