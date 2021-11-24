#pragma once
#include "cMesh.h"
#include <Physics/cParticle.h>

class cEntity {
public:
	cEntity(cMesh* _mesh);
	virtual ~cEntity();
	cMesh* mesh;
	cMesh* debugMesh;
	cParticle* particle;
};