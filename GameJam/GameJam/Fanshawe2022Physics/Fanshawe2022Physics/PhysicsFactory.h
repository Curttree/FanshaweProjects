#pragma once

#include <Fanshawe2022Physics/interfaces/iPhysicsFactory.h>

namespace gdp2022Physics
{
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory();
		virtual ~PhysicsFactory();

		virtual iPhysicsWorld* CreateWorld();

		virtual iRigidBody* CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape);
	};
}
