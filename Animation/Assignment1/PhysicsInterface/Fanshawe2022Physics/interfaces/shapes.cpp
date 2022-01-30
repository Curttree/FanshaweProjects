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
}