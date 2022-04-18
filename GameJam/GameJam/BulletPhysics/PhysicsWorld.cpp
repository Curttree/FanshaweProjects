#include "PhysicsWorld.h"

#include "PhysicsConversion.h"

// BULLET

namespace gdp2022Physics
{
	PhysicsWorld::PhysicsWorld()
	{
		btVector3 worldMin(-1000, -1000, -1000);
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
}