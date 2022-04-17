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

#ifdef USE_SOFT_BODY_WORLD
		mSoftBodySolver = new btDefaultSoftBodySolver();
		mSoftRigidWorld = new btSoftRigidDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration, mSoftBodySolver);
		mVehicleRaycaster = new VehicleRaycaster(new btDefaultVehicleRaycaster(mSoftRigidWorld));
#else
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
#endif

		mVersion = "Bullet";
		printf("Bullet Physics World\n");

	}

	PhysicsWorld::~PhysicsWorld()
	{
		// TODO:
		// Remove rigid bodies from the dynamics world
		// remove soft bodies from the soft rigid world
		// delete soft bodies
		// delete motion states
		// delete rigid bodies
		// Delete shapes.

#ifdef USE_SOFT_BODY_WORLD
		delete mSoftRigidWorld;
		delete mSoftBodySolver;
#else
		delete mDynamicsWorld;
#endif
		delete mSolver;
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mBroadphase;
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
#ifdef USE_SOFT_BODY_WORLD
		mSoftRigidWorld->setGravity(btGravity);
#else
		mDynamicsWorld->setGravity(btGravity);
#endif
	}

	void PhysicsWorld::RegisterCollisionListener(iCollisionListener* listener)
	{
		// TODO:
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		if (std::find(mBodies.begin(), mBodies.end(), body) != mBodies.end())
			return;

		if (body->GetBodyType() == gdp2022Physics::eBodyType::SOFT_BODY)
		{
#ifdef USE_SOFT_BODY_WORLD
			printf("PhysicsWorld.AddBody: [Warning] Unimplemented adding soft body to mSoftRigidWorld!\n");
			mSoftRigidWorld->addSoftBody(CastBulletSoftBody(body));
#else
			printf("PhysicsWorld.AddBody: [Error] Unable to add a soft body to a dynamics world!\n");
			return;
#endif
		}

		// TODO:
		// May want to add 2 more vectors
		// vector<SoftBody*> mSoftBodies;
		// vector<RigidBody*> mRigidBodies;
		mBodies.push_back(body);

#ifdef USE_SOFT_BODY_WORLD
		mSoftRigidWorld->addRigidBody(CastBulletRigidBody(body));
#else
		mDynamicsWorld->addRigidBody(CastBulletRigidBody(body));
#endif
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		// TODO:
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