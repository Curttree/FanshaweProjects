#pragma once

#include <extern/glm/glm.hpp>
#include "iCollisionListener.h"
#include "iCollisionBody.h"
#include "../../../BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

namespace gdp2022Physics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() { }

		virtual void SetGravity(const glm::vec3& gravity) = 0;
		virtual void AddBody(iCollisionBody* body) = 0;
		virtual void RemoveBody(iCollisionBody* body) = 0;
		virtual void TimeStep(float dt) = 0; 
		virtual iCollisionBody* RayHit(glm::vec3 start, glm::vec3 end) = 0;

		virtual void* getDynamicsWorld() = 0;

	protected:
		iPhysicsWorld() { }

	private:
		iPhysicsWorld(const iPhysicsWorld&) { }
		iPhysicsWorld& operator=(const iPhysicsWorld&) { return *this; }
	};
}
