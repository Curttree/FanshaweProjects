#include "shapes.h"

#include <extern/glm/glm.hpp>

namespace gdp2022Physics
{
	#pragma region Box
	BoxShape::BoxShape(const glm::vec3& halfExtents)
		: iShape(eShapeType::Box)
		, m_HalfExtents(halfExtents)
	{
	}

	BoxShape::~BoxShape()
	{}

	BoxShape* BoxShape::Cast(iShape* shape)
	{
		return dynamic_cast<BoxShape*>(shape);
	}
	#pragma endregion

	#pragma region Cylinder

	CylinderShape::CylinderShape(const glm::vec3& halfExtents)
		: iShape(eShapeType::Cylinder)
		, m_HalfExtents(halfExtents)
	{
	}

	CylinderShape::~CylinderShape()
	{}

	CylinderShape* CylinderShape::Cast(iShape* shape)
	{
		return dynamic_cast<CylinderShape*>(shape);
	}

	#pragma endregion

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