#pragma once

#include "PhysicsFactory.h"

#include "PhysicsWorld.h"
#include "cRigidBody.h"

namespace gdp2022Physics
{
	PhysicsFactory::PhysicsFactory()
		: iPhysicsFactory()
	{
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
		return new cRigidBody(desc, shape);
	}
}
