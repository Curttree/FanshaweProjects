#include "Conversion.h"

#include <Fanshawe2022Physics/interfaces/shapes.h>
#include "RigidBody.h"
#include "SoftBody.h"

namespace gdp2022Physics
{

	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out)
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
		out->setW(in.w);
	}

	void CastBulletVector3(const glm::vec3& in, btVector3* out)			  
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
	}

	void CastBulletScalar(const float in, btScalar* out)				  
	{
		*out = in;
	}

	void CastGLMQuaternion(const btQuaternion& in, glm::quat* out)		  
	{
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
		CastFloat(in.w(), &out->w);
	}

	void CastGLMVector3(const btVector3& in, glm::vec3* out)			  
	{
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
	}

	void CastTransform(const btTransform& in, Transform* out)
	{
		CastGLMQuaternion(in.getRotation(), &out->rotation);
		btVector3 position = in.getBasis() * in.getOrigin();
		CastGLMVector3(position, &out->position);
	}

	void CastGLMPosition(const btTransform& in, glm::vec3* out)
	{
		CastFloat(in.getOrigin().x(), &out->x);
		CastFloat(in.getOrigin().y(), &out->y);
		CastFloat(in.getOrigin().z(), &out->z);
	}

	void CastFloat(const btScalar& in, float* out)
	{
		*out = in;
	}

	btRigidBody* CastBulletRigidBody(iCollisionBody* body)
	{
		return dynamic_cast<RigidBody*>(body)->GetBulletBody();
	}

	btSoftBody* CastBulletSoftBody(iCollisionBody* body)
	{
		return dynamic_cast<SoftBody*>(body)->GetBulletBody();
	}

	btCollisionShape* CastBulletShape(iShape* shape)
	{
		switch (shape->GetShapeType())
		{
		case eShapeType::Box:
		{
			BoxShape* box = BoxShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(box->GetHalfExtents(), &halfExtents);

			btBoxShape* btBox = new btBoxShape(halfExtents);

			return btBox;
		}
		break;

		case eShapeType::Cylinder:
		{
			CylinderShape* cylinder = CylinderShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(cylinder->GetHalfExtents(), &halfExtents);

			btCylinderShape* btCylinder = new btCylinderShape(halfExtents);

			return btCylinder;
		}
		break;

		case eShapeType::Plane:
		{
			PlaneShape* plane = PlaneShape::Cast(shape);

			btVector3 normal;
			btScalar dot;

			CastBulletVector3(plane->GetNormal(), &normal);
			CastBulletScalar(plane->GetDotProduct(), &dot);

			btStaticPlaneShape* btPlane = new btStaticPlaneShape(normal, dot);

			return btPlane;
		}
		break;

		case eShapeType::Sphere:
		{
			SphereShape* sphere = SphereShape::Cast(shape);

			btScalar radius;

			CastBulletScalar(sphere->GetRadius(), &radius);

			btSphereShape* btSphere = new btSphereShape(radius);

			return btSphere;
		}
		break;

		default:
			break;
		}
	}
}