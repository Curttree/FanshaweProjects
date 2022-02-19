#include "PhysicsWorld.h"

namespace gdp2022Physics
{

	PhysicsWorld::PhysicsWorld()
		: iPhysicsWorld()
		, m_CollisionListener(0)
	{
		m_CollisionHandler = new CollisionHandler();
	}

	PhysicsWorld::~PhysicsWorld()
	{
		for (unsigned int i = 0; i < m_RigidBodies.size(); ++i)
		{
			delete m_RigidBodies[i];
		}
		m_RigidBodies.clear();
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		m_Gravity = gravity;
	}

	void PhysicsWorld::RegisterCollisionListener(iCollisionListener* listener)
	{
		m_CollisionListener = listener;
	}

	void PhysicsWorld::AddRigidBody(iRigidBody* body)
	{
		if (body == 0)
			return;

		m_RigidBodies.push_back(dynamic_cast<cRigidBody*>(body));
	}

	void PhysicsWorld::RemoveRigidBody(iRigidBody* body)
	{
		cRigidBody* rigidBody = dynamic_cast<cRigidBody*>(body);
		std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), rigidBody);
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		int rigidBodyCount = m_RigidBodies.size();

		// Step 0 : Updating Stuff
		for (int i = 0; i < rigidBodyCount; ++i)
		{
			if (m_RigidBodies[i]->IsStatic() == false)
			{
				m_RigidBodies[i]->SetGravityAcceleration(m_Gravity);
				m_RigidBodies[i]->UpdateAcceleration();
			}
		}


		// Step 1 : Verlet Positions
		for (int i = 0; i < rigidBodyCount; ++i)
		{
			if (m_RigidBodies[i]->IsStatic() == false)
			{
				m_RigidBodies[i]->VerletStep1(dt);
			}
		}

		// Implement Collision Resolution
		std::vector<CollidingBodies> collidingBodies;
		m_CollisionHandler->Collide(dt, m_RigidBodies, collidingBodies);

		// Step 2 : Verlet
		for (int i = 0; i < rigidBodyCount; ++i)
		{
			if (m_RigidBodies[i]->IsStatic() == false)
			{
				m_RigidBodies[i]->VerletStep2(dt);
				m_RigidBodies[i]->ApplyDamping(dt * 0.5f);
				m_RigidBodies[i]->KillForces();
			}
		}

		// TODO: Determine if we should move this after Verlet step 2.
		// Step 3 : Verlet
		for (int i = 0; i < rigidBodyCount; ++i)
		{
			if (m_RigidBodies[i]->IsStatic() == false)
			{
				m_RigidBodies[i]->VerletStep3(dt);
				m_RigidBodies[i]->ApplyDamping(dt * 0.5f);
			}
		}

		// For each colliding body pair, tell the collision listener
		// the two bodies that have collided.
		for (int i = 0; i < collidingBodies.size(); ++i)
		{
			// Notify on collision between the two bodies.
			m_CollisionListener->Collision(collidingBodies[i].BodyA, collidingBodies[i].BodyB);
		}
	}
}