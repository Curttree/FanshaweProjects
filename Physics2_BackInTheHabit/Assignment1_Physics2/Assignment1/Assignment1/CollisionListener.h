#pragma once

#include <Fanshawe2022Physics/interfaces/iCollisionListener.h>

namespace gdp2022Physics
{
class CollisionListener : public gdp2022Physics::iCollisionListener
{
public:
	CollisionListener() { }
	virtual ~CollisionListener() { }

	virtual void Collision(iRigidBody* bodyA, iRigidBody* bodyB);

};
}