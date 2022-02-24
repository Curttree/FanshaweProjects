#pragma once
#include "cEntity.h"

class cAIPlayer : public cEntity {
public:
	cAIPlayer(cMesh* _mesh, float _debugScale, float _density = 1.f);
	virtual void Update(float deltaTime);
};