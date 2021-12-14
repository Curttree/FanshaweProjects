#pragma once
#include "cMesh.h"
#include "iEntity.h"
#include <Physics/cParticle.h>
#include <Physics/cWorldSpace.h>

class cEntity : public iEntity {
public:
	cEntity(cMesh* _mesh, float _debugScale, float _density = 1.f);
	virtual ~cEntity();
	cMesh* mesh;
	cMesh* debugMesh;
	cParticle* particle;
	virtual void Update(float deltaTime);
	virtual cMesh* GetDebugMesh();
	float particleScale = 10.f;
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	glm::vec3 velocity;
	cEntity();
};