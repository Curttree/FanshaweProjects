#pragma once
#include "iFuse.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class cApexFuse : public iFuse {
	public:
		cApexFuse(bool* _hitApex);
		~cApexFuse();
		virtual bool isReadyForStageTwo();
	private:
		bool* hitApex;
};