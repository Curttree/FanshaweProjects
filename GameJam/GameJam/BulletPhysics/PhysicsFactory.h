#pragma once

#include <Fanshawe2022Physics/interfaces/iPhysicsFactory.h>
#include <Fanshawe2022Physics/interfaces/cSoftBodyDesc.h>
#include <Fanshawe2022Physics/interfaces/iSoftBody.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <string>

namespace gdp2022Physics
{
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory();
		virtual ~PhysicsFactory();

		virtual iPhysicsWorld* CreateWorld();

		virtual iRigidBody* CreateRigidBody(const cRigidBodyDesc& desc, iShape* shape);
		virtual iSoftBody* CreateSoftBody(const cSoftBodyDesc& desc);
	private:
		std::string mVersion;
	};
}
