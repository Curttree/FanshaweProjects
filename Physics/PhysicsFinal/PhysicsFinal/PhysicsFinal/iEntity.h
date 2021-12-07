#pragma once
#include "cMesh.h"
#include <Physics/cParticle.h>
#include <Physics/cWorldSpace.h>

class iEntity {
public:
	virtual void Update(float deltaTime) = 0;
	virtual cMesh* GetDebugMesh() = 0;
};