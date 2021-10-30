#include "cApexFuse.h"

cApexFuse::cApexFuse(nPhysics::cParticle* _attachedParticle) {
	attachedParticle = _attachedParticle;
}

bool cApexFuse::isReadyForNextStage() {
	return glm::dot(attachedParticle->GetVelocity(), worldSpace->axes[1]) <= 0;
}