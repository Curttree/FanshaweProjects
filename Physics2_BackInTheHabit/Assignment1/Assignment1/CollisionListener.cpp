#include "CollisionListener.h"
#include "globals.h"

namespace gdp2022Physics
{
	void CollisionListener::Collision(iRigidBody* bodyA, iRigidBody* bodyB)
	{
		if (bodyA->GetFriendlyName() == "Ball" && bodyB->GetFriendlyName() == "Ball") {
			::g_pGameEngine->audioManager.PlayAudio(bodyA->GetPosition());
		}
	}
}