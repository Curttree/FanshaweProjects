#pragma once
#include "cMesh.h"
#include <Fanshawe2022Physics/cRigidBody.h>

class cEntity {
public:
	cEntity();
	virtual ~cEntity();

	cMesh* mesh;
	gdp2022Physics::iRigidBody* rigidBody;
};