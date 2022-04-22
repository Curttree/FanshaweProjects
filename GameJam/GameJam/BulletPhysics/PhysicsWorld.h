#pragma once

#include <Fanshawe2022Physics/interfaces/iPhysicsWorld.h>
#include <vector>
#include <string>

// Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>

namespace gdp2022Physics
{
	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void TimeStep(float dt);

		virtual void SetGravity(const glm::vec3& gravity);

		virtual void AddBody(iCollisionBody* body);
		virtual void RemoveBody(iCollisionBody* body);

		virtual iCollisionBody* RayHit(glm::vec3 start, glm::vec3 end);

		virtual const std::string& GetVersion();

		void* getDynamicsWorld() { return mDynamicsWorld; };

	private:
		btBroadphaseInterface* mBroadphase = 0;
		btDefaultCollisionConfiguration* mCollisionConfiguration = 0;
		btCollisionDispatcher* mDispatcher = 0;
		btSequentialImpulseConstraintSolver* mSolver = 0;

		btDiscreteDynamicsWorld* mDynamicsWorld = 0;

		std::vector<iCollisionBody*> mBodies;
		std::vector<btRaycastVehicle*> mVehicles;

		std::string mVersion;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) { return *this; }
	};
}