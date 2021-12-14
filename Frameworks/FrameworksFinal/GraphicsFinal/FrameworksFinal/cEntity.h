#pragma once
#include "cMesh.h"
#include "iEntity.h"
#include "iMessage.h"
#include <Physics/cParticle.h>
#include <Physics/cWorldSpace.h>

class cEntity : public iEntity, public iMessage {
public:
	cEntity(cMesh* _mesh, float _debugScale, float _density = 1.f);
	virtual ~cEntity();
	cMesh* mesh;
	cMesh* debugMesh;
	cParticle* particle;
	virtual void Update(float deltaTime);
	virtual cMesh* GetDebugMesh();
	float particleScale = 10.f;

	// Via iMessage
	virtual bool RecieveMessage(sMessage theMessage);
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse);
	virtual bool SetReciever(iMediator* pTheReciever);
protected:
	cWorldSpace* worldSpace = cWorldSpace::Instance();
	glm::vec3 velocity;
	cEntity();
};