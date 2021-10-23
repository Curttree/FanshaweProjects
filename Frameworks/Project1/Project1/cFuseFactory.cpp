#include "cApexFuse.h"
#include "cTimedFuse.h"
#include "cFuseFactory.h"

cFuseFactory* cFuseFactory::_instance = 0;
cFuseFactory::cFuseFactory() {

}

cFuseFactory* cFuseFactory::Instance() {
	if (_instance == 0) {
		_instance = new cFuseFactory;
	}

	return _instance;
}

iFuse* cFuseFactory::createFuse(int type, nPhysics::cParticle* attachedParticle, float maxTime) {
	iFuse* fuse = 0;
	switch (type) {
	case(1):
		fuse = new cApexFuse(attachedParticle);
		break;
	case(2):
		fuse = new cTimedFuse(maxTime);
		break;
	default:
		// If not defined, return a fuse that will trigger immediately.
		fuse = new cTimedFuse(0.f);
		break;
	}
	return fuse;
}