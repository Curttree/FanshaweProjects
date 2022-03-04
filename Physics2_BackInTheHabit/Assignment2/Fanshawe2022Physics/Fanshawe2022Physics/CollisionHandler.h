#pragma once

#include <vector>

#include <Fanshawe2022Physics/interfaces/shapes.h>

namespace gdp2022Physics
{
	class cRigidBody;	// Forward declaration of rigid body class
	struct CollidingBodies
	{
		CollidingBodies(cRigidBody* bodyA, cRigidBody* bodyB)
			: BodyA(bodyA)
			, BodyB(bodyB)
		{}
		cRigidBody* BodyA;
		cRigidBody* BodyB;
	};

	class CollisionHandler
	{
	public:
		CollisionHandler();
		virtual ~CollisionHandler();

		bool CollideSphereSphere(float deltaTime, cRigidBody* a_body, SphereShape* a_sphere,
			cRigidBody* b_body, SphereShape* b_sphere);

		bool CollideSpherePlane(float deltaTime, cRigidBody* sphere, SphereShape* sphereShape,
			cRigidBody* plane, PlaneShape* planeShape);

		bool CollideSphereAABB(float deltaTime, cRigidBody* sphere, SphereShape* sphereShape,
			cRigidBody* aabb, AABBShape* aabbShape);

		bool CollideAABBPlane(float deltaTime, cRigidBody* aabb, AABBShape* aabbShape,
			cRigidBody* plane, PlaneShape* planeShape);

		bool CollideAABBAABB(float deltaTime, cRigidBody* a_aabb, AABBShape* a_aabbShape,
			cRigidBody* b_aabb, AABBShape* b_aabbShape);

		void Collide(float deltaTime, std::vector<cRigidBody*>& bodies, std::vector<CollidingBodies>& collidingBodies);
	
	private:
		
		float elasticity = 0.33f;	// This can be changed from a value between 0.0 and 1.0.
	};

}
