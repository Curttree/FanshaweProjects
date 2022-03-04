#include "shapes.h"

#include <extern/glm/glm.hpp>

namespace gdp2022Physics
{
	#pragma region Sphere
	SphereShape::SphereShape(float radius)
		: iShape(eShapeType::Sphere)
		, radius(radius)
	{}

	SphereShape::~SphereShape()
	{}

	float SphereShape::GetRadius() const
	{
		return radius;
	}

	SphereShape* SphereShape::Cast(iShape* shape)
	{
		return dynamic_cast<SphereShape*>(shape);
	}
	#pragma endregion

	#pragma region Plane
	PlaneShape::PlaneShape(float dotProduct, const glm::vec3& normal)
		: iShape(eShapeType::Plane)
		, m_DotProduct(dotProduct)
		, m_Normal(normal)
	{
	}

	PlaneShape* PlaneShape::Cast(iShape* shape)
	{
		return dynamic_cast<PlaneShape*>(shape);
	}
	#pragma endregion

	#pragma region AABB

	AABBShape::AABBShape(const glm::vec3& minimumValues, const glm::vec3& maximumValues)
		: iShape(eShapeType::AABB)
		, min(minimumValues)
		, max(maximumValues)
	{
	}
	AABBShape* AABBShape::Cast(iShape* shape)
	{
		return dynamic_cast<AABBShape*>(shape);
	}
	#pragma endregion
}