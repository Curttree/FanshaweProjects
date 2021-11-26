#pragma once
#include "cMesh.h"
#include <Physics/cParticle.h>
#include <Physics/cWorldSpace.h>

class cEntity {
public:
	cEntity(cMesh* _mesh);
	virtual ~cEntity();
	cMesh* mesh;
	cMesh* debugMesh;
	cParticle* particle;
	void Update(float deltaTime);
	float particleScale = 10.f;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
};