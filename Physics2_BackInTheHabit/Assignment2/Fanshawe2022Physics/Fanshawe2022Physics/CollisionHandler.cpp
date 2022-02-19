#include "CollisionHandler.h"

#include "cRigidBody.h"
#include <extern/glm/geometric.hpp>
#include <iostream>

namespace gdp2022Physics
{
	CollisionHandler::CollisionHandler() {};
	CollisionHandler::~CollisionHandler() {};

	// Helper Method declarations. These are defined below.
	glm::vec3 ClosestPtPointPlane(const glm::vec3& pt, const glm::vec3& norm, float dotProduct); 
	bool TestMovingSpherePlane(const glm::vec3& a, const glm::vec3& b, float r, const glm::vec3& norm, float dotProduct);
	bool TestMovingSphereSphere(
		const glm::vec3& s0Center, const float s0Radius,
		const glm::vec3& s1Center, const float s1Radius,
		const glm::vec3& v0, const glm::vec3& v1, float& t);
	glm::vec3 ProjectOn(glm::vec3 b, glm::vec3 a);

	bool CollisionHandler::CollideSphereSphere(float deltaTime, cRigidBody* a_body, SphereShape* a_sphere, cRigidBody* b_body, SphereShape* b_sphere) {
		if (a_body->IsStatic() && b_body->IsStatic()) {
			return false;
		}

		glm::vec3 vecA = a_body->position - a_body->previousPosition;
		glm::vec3 vecB = b_body->position - b_body->previousPosition;

		float collisionTime = 0.f;

		if (!TestMovingSphereSphere(a_body->previousPosition, a_sphere->GetRadius(),
									b_body->previousPosition, b_sphere->GetRadius(),
									vecA, vecB, collisionTime)) 
		{
			// No current or future collision. We can exit.
			return false;
		}

		if (collisionTime > deltaTime) {
			// No collision this update step. Nothing to do this step.
			return false;
		}

		float fractDt = collisionTime / (glm::length(vecA) + glm::length(vecB));
		fractDt = glm::clamp(fractDt, 0.f, 1.f);
		float revDt = (1.f - fractDt) * deltaTime;
		a_body->VerletStep1(-revDt);
		b_body->VerletStep1(-revDt);

		float combinedMass = a_body->mass + b_body->mass;

		// Factors are based on the mass of the other object (smaller object is impacted more by collision) 
		float aFactor = b_body->mass / combinedMass;
		float bFactor = a_body->mass / combinedMass;

		glm::vec3 vecAB = b_body->position - a_body->position;
		float lengthAB = glm::length(vecAB);
		float penetration = lengthAB - (a_sphere->GetRadius() + b_sphere->GetRadius());

		if (penetration < std::numeric_limits<float>::epsilon()) {
			glm::vec3 vecPenetration = vecAB;
			vecPenetration = glm::normalize(vecPenetration);
			vecPenetration *= -penetration;

			if (!a_body->isStatic) {
				b_body->position += vecPenetration * bFactor;
			}

			if (!b_body->isStatic) {
				a_body->position -= vecPenetration * aFactor;
			}

			vecAB = b_body->position - a_body->position;
			lengthAB = glm::length(vecAB);
		}
		vecAB /= lengthAB;

		// Momentum Calculations : Momentum = Mass * Velocity
		glm::vec3 a_momentum = a_body->linearVelocity * a_body->mass;
		glm::vec3 b_momentum = b_body->linearVelocity * b_body->mass;
		glm::vec3 total_momentum = a_momentum + b_momentum;

		a_momentum = total_momentum * aFactor;
		b_momentum = total_momentum * bFactor;

		glm::vec3 a_elasticMomenum = vecAB * (glm::length(a_momentum) * elasticity);
		glm::vec3 a_inelasticMomenum = vecAB * (glm::length(a_momentum) * (1.f - elasticity));

		glm::vec3 b_elasticMomenum = vecAB * (glm::length(b_momentum) * elasticity) * (-1.f);
		glm::vec3 b_inelasticMomenum = vecAB * (glm::length(b_momentum) * (1.f - elasticity));

		a_body->linearVelocity -= (a_elasticMomenum + a_inelasticMomenum) * (a_body->invMass * a_body->restitution);
		b_body->linearVelocity += (b_elasticMomenum + b_inelasticMomenum) * (b_body->invMass * b_body->restitution);

		a_body->VerletStep1(revDt);
		b_body->VerletStep1(revDt);

		return true;
	}

	bool CollisionHandler::CollideSpherePlane(float deltaTime, cRigidBody* sphere, SphereShape* sphereShape,
		cRigidBody* plane, PlaneShape* planeShape) {

		if (!TestMovingSpherePlane(sphere->previousPosition, sphere->position, sphereShape->GetRadius(),
			planeShape->GetNormal(), planeShape->GetDotProduct()))
		{
			// No current or future collision. We can exit.
			return false;
		}
		glm::vec3 closestPoint = ClosestPtPointPlane(sphere->position, planeShape->GetNormal(), planeShape->GetDotProduct());
		glm::vec3 penetrationVec = closestPoint - sphere->position;
		float penetration = glm::length(penetrationVec);

		float velocityLength = glm::length(sphere->linearVelocity);

		if (velocityLength > 0.000001f)
		{

			float velocity = glm::length(sphere->linearVelocity);

			// fractDt is the overlap "ratio" of the timestep
			float fractDt = sphereShape->GetRadius() * ((sphereShape->GetRadius() / penetration) - 1.0f) / velocity;

			// partialDt is the part of the dt contributed to the overlap
			float partialDt = (1.f - fractDt) * deltaTime;

			// Backing up the sphere so it should be just touching the plane
			sphere->VerletStep1(-partialDt);

			// Bouncing off
			glm::vec3 reflect = glm::reflect(sphere->linearVelocity, planeShape->GetNormal());

			glm::vec3 prevVelocity = sphere->linearVelocity;
			sphere->linearVelocity = reflect;

			glm::vec3 impactComponent = ProjectOn(sphere->linearVelocity, planeShape->GetNormal());
			glm::vec3 impactTangent = prevVelocity - impactComponent;

			glm::vec3 relativePoint = glm::normalize(closestPoint - sphere->position) * sphereShape->GetRadius();
			float surfaceVelocity = sphereShape->GetRadius() * glm::length(sphere->angularVelocity);

			glm::vec3 angularDirection = glm::cross(impactTangent, sphere->angularVelocity);

			if (glm::dot(impactTangent, planeShape->GetNormal()) >= 0)
			{
				sphere->ApplyImpulseAtPoint(-2.0f * impactComponent * sphere->mass, relativePoint);
			}
			else
			{
				sphere->angularVelocity *= plane->friction;
				sphere->ApplyForceAtPoint(angularDirection, relativePoint);
			}

			sphere->UpdateAcceleration();

			sphere->VerletStep1(partialDt);

			closestPoint = ClosestPtPointPlane(sphere->position, planeShape->GetNormal(), planeShape->GetDotProduct());
			penetrationVec = closestPoint - sphere->position;
			penetration = glm::length(penetrationVec);
			if (penetration < sphereShape->GetRadius())
			{
				sphere->position += planeShape->GetNormal() * (sphereShape->GetRadius() - penetration);
				float velDotNorm = glm::dot(sphere->linearVelocity, planeShape->GetNormal());

				if (velDotNorm < 0.f)
				{
					sphere->linearVelocity -= planeShape->GetNormal() * velDotNorm;
				}
			}
			else
			{
				// We are no longer overlapping. Reduce the velocity.
				sphere->linearVelocity *= sphere->restitution;
			}
		}

		return true;
	}

	void CollisionHandler::Collide(
		float deltaTime, std::vector<cRigidBody*>& bodies, std::vector<CollidingBodies>& collidingBodies)
	{
		iShape* shapeA;
		iShape* shapeB;
		int bodyCount = bodies.size();
		for (int idxA = 0; idxA < bodyCount - 1; idxA++)
		{
			for (int idxB = idxA + 1; idxB < bodyCount; idxB++)
			{
				bool collision = false;

				cRigidBody* bodyA = bodies[idxA];
				cRigidBody* bodyB = bodies[idxB];

				shapeA = bodyA->GetShape();
				shapeB = bodyB->GetShape();

				if (shapeA->GetShapeType() == eShapeType::Sphere)
				{
					if (shapeB->GetShapeType() == eShapeType::Sphere)
					{
						if (CollideSphereSphere(deltaTime,
							bodyA, SphereShape::Cast(shapeA),
							bodyB, SphereShape::Cast(shapeB)))
						{
							collision = true;
						}
					}
					else if (shapeB->GetShapeType() == eShapeType::Plane)
					{
						if (CollideSpherePlane(deltaTime,
							bodyA, SphereShape::Cast(shapeA),
							bodyB, PlaneShape::Cast(shapeB)))
						{
							collision = true;
						}
					}
				}
				else if (shapeA->GetShapeType() == eShapeType::Plane)
				{
					if (shapeB->GetShapeType() == eShapeType::Sphere)
					{
						if (CollideSpherePlane(deltaTime,
							bodyB, SphereShape::Cast(shapeB),
							bodyA, PlaneShape::Cast(shapeA)))
						{
							collision = true;
						}
					}
					else if (shapeB->GetShapeType() == eShapeType::Plane)
					{
						// Not worrying about plane/plane collisions.
					}
				}

				if (collision)
				{
					//Add collision to vector
					collidingBodies.push_back(CollidingBodies(bodyA, bodyB));
				}
			}
		}
	}


	#pragma region Helper Methods
	glm::vec3 ProjectOn(glm::vec3 b, glm::vec3 a)
	{
		float multiplier = ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)) / ((b.x * b.x) + (b.y * b.y) + (b.z * b.z));
		return glm::vec3(b.x * multiplier, b.y * multiplier, b.z * multiplier);
	}

	// Page 224 Chapter 5 Basic Primitive Tests
	// Christer Ericson - Real-time Collision Detection
	bool TestMovingSphereSphere(
		const glm::vec3& s0Center, const float s0Radius,
		const glm::vec3& s1Center, const float s1Radius,
		const glm::vec3& v0, const glm::vec3& v1, float& t)
	{
		glm::vec3 s = s1Center - s0Center;	// Vector between sphere centers
		glm::vec3 v = v1 - v0;				// Relative motion of s1 with respect to stationary s0
		float r = s1Radius + s0Radius;		// Sum of sphere radii
		float c = glm::dot(s, s) - r * r;

		if (c < 0.0f) {
			// Already intersecting, not moving towards a collision
			t = 0.0f;
			return true;
		}

		float a = glm::dot(v, v);
		if (a < std::numeric_limits<float>::epsilon())
			return false;		// Spheres not moving relative to each other

		float b = glm::dot(v, s);
		if (b >= 0.0f)
			return false;		// Spherse not moving towards each other

		float d = (b * b) - (a * c);
		if (d < 0.0f)
			return false;		// No real-valued root, spheres do not intersect

		t = (-b - sqrt(d)) / a;
		return true;
	}

	// Page 222 Chapter 5 Basic Primitive Tests
	// Christer Ericson - Real-time Collision Detection
	bool TestMovingSpherePlane(const glm::vec3& a, const glm::vec3& b, float r, const glm::vec3& norm, float dotProduct)
	{
		// Get distance for both a and b from the plane
		float adist = glm::dot(a, norm) - dotProduct;
		float bdist = glm::dot(b, norm) - dotProduct;

		// Intersects if on different sides of plane (distances have different signs)
		if (adist * bdist < 0.0f)
			return true;

		// Intersects if start or end position within radius from plane
		if (abs(adist) <= r || abs(bdist) < r)
			return true;

		// No intersection
		return false;
	}


	// Page 127 Chapter 5 Basic Primitive Tests
	// Christer Ericson - Real-time Collision Detection
	glm::vec3 ClosestPtPointPlane(const glm::vec3& pt, const glm::vec3& norm, float dotProduct)
	{
		float t = glm::dot(norm, pt) - dotProduct;
		return pt - t * norm;
	}
	#pragma endregion
}