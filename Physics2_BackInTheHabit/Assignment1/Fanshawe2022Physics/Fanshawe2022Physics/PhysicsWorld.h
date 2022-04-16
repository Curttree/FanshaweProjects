#pragma once

#include <Fanshawe2022Physics/interfaces/iPhysicsWorld.h>
#include <vector>

#include "cRigidBody.h"
#include "CollisionHandler.h"


namespace gdp2022Physics
{
	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void SetGravity(const glm::vec3& gravity);
		virtual void RegisterCollisionListener(iCollisionListener* listener);
		virtual void AddRigidBody(iRigidBody* body);
		virtual void RemoveRigidBody(iRigidBody* body);
		virtual void TimeStep(float dt);
	private:
		glm::vec3 m_Gravity = glm::vec3(0.f);
		std::vector<cRigidBody*> m_RigidBodies;
		CollisionHandler* m_CollisionHandler = 0;
		iCollisionListener* m_CollisionListener = 0;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) { return *this; }
	};
}