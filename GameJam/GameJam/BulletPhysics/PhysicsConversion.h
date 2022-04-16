#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletSoftBody/btSoftBody.h>

#include <Fanshawe2022Physics/interfaces/iCollisionBody.h>
#include <Fanshawe2022Physics/interfaces/iRigidBody.h>
#include <Fanshawe2022Physics/interfaces/iSoftBody.h>
#include <Fanshawe2022Physics/interfaces/iShape.h>

#include <extern/glm/glm.hpp>
#include <extern/glm/gtx/quaternion.hpp>

namespace gdp2022Physics
{
	struct Transform
	{
		glm::vec3 position;
		glm::quat rotation;
	};

	// GLM -> Bullet
	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out);
	void CastBulletVector3(const glm::vec3& in, btVector3* out);
	void CastBulletScalar(const float in, btScalar* out);

	// Bullet -> GLM
	void CastGLMQuaternion(const btQuaternion& in, glm::quat* out);
	void CastGLMVector3(const btVector3& in, glm::vec3* out);
	void CastGLMMatrix4(const btTransform& in, glm::mat4* out);
	void CastGLMPosition(const btTransform& in, glm::vec3* out);
	void CastFloat(const btScalar& in, float* out);
	void CastTransform(const btTransform& in, Transform* out);

	// Interface -> Bullet
	btRigidBody* CastBulletRigidBody(iCollisionBody* body);
	btSoftBody* CastBulletSoftBody(iCollisionBody* body);
	btCollisionShape* CastBulletShape(iShape* shape);
}