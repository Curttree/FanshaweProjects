#include "PhysicsWorld.h"

#include "PhysicsConversion.h"
#include <iostream>

// BULLET

namespace gdp2022Physics
{
	PhysicsWorld::PhysicsWorld()
	{
		btVector3 worldMin(-1000, 0, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		mBroadphase = new btAxisSweep3(worldMin, worldMax);

		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mSolver = new btSequentialImpulseConstraintSolver();
		
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

		mVersion = "Bullet";
		printf("Bullet Physics World\n");

	}

	PhysicsWorld::~PhysicsWorld()
	{
		// TODO:
		// Remove rigid bodies from the dynamics world
		// delete motion states
		// delete rigid bodies
		// Delete shapes.

		delete mDynamicsWorld;
		delete mSolver;
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mBroadphase;
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
		mDynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		if (std::find(mBodies.begin(), mBodies.end(), body) != mBodies.end())
			return;

		if (body->GetBodyType() == gdp2022Physics::eBodyType::SOFT_BODY)
		{
			printf("PhysicsWorld.AddBody: [Error] Unable to add a soft body to a dynamics world!\n");
			return;
		}

		mBodies.push_back(body);

		mDynamicsWorld->addRigidBody(CastBulletRigidBody(body));
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		if (std::find(mBodies.begin(), mBodies.end(), body) == mBodies.end())
			return;

		mBodies.erase(std::find(mBodies.begin(), mBodies.end(), body));

		mDynamicsWorld->removeRigidBody(CastBulletRigidBody(body));

		delete body;
		body = 0;
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		mDynamicsWorld->stepSimulation(dt, 10);
	}

	const std::string& PhysicsWorld::GetVersion()
	{
		return mVersion;
	}

	iCollisionBody* PhysicsWorld::RayHit(glm::vec3 start, glm::vec3 end) {
		btVector3 b_start;
		btVector3 b_end;
		CastBulletVector3(start, &b_start);
		CastBulletVector3(end, &b_end);
		btCollisionWorld::ClosestRayResultCallback RayCallback(b_start, b_end);
		mDynamicsWorld->rayTest(b_start, b_end, RayCallback);
		if (RayCallback.hasHit()) {
			const btCollisionObject* hit = RayCallback.m_collisionObject; 
			gdp2022Physics::iRigidBody* rigid;
			for (iCollisionBody* body : mBodies) {
				if (body->GetBodyType() == RIGID_BODY && CastBulletRigidBody(body) == btRigidBody::upcast(hit)) {
					return body;
				}
				//Ignore other body types..just like the fashion industry for decades.
			}
		}
		return 0;
	}
}