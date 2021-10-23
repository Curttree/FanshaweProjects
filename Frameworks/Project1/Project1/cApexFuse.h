#pragma once
#include "iFuse.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class cApexFuse : public iFuse {
	public:
		cApexFuse(nPhysics::cParticle* _attachedParticle);
		virtual bool isReadyForNextStage();
	private:
		nPhysics::cParticle* attachedParticle;
};