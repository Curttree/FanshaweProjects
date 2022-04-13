#include "PhysicsFactory.h"

#include <cassert>

#include "Conversion.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "SoftBody.h"

// BULLET

namespace gdp2022Physics
{
	PhysicsFactory::PhysicsFactory()
		: iPhysicsFactory()
		, mVersion("Bullet")
	{
		printf("Bullet Physics Factory\n");
	}

	PhysicsFactory::~PhysicsFactory()
	{
	}

	iPhysicsWorld* PhysicsFactory::CreateWorld()
	{
		return new PhysicsWorld();
	}

	iRigidBody* PhysicsFactory::CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape)
	{
		return new RigidBody(desc, shape);
	}

	iSoftBody* PhysicsFactory::CreateSoftBody(const cSoftBodyDesc& desc)
	{
		return 0;// new SoftBody(desc);
	}
}
