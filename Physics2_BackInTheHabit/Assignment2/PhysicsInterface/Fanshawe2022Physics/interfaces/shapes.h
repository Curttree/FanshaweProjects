#pragma once

#include "iShape.h"
#include <extern/glm/vec3.hpp>

namespace gdp2022Physics
{
	class SphereShape : public iShape
	{
	public:
		SphereShape(float radius);
		virtual ~SphereShape();

		float GetRadius() const;
		static SphereShape* Cast(iShape* shape);

	protected:
		SphereShape(eShapeType shapeType)
			: iShape(shapeType) {}

	private:
		float radius = 1.f;
		SphereShape(const SphereShape&) : iShape(eShapeType::Sphere) {}
		SphereShape& operator=(const SphereShape&) { return *this; }
	};

	class PlaneShape : public iShape
	{
	public:
		PlaneShape(float dotProduct, const glm::vec3& normal);

		float GetDotProduct() const { return m_DotProduct; }
		const glm::vec3& GetNormal() { return m_Normal; }

		static PlaneShape* Cast(iShape* shape);

	private:
		glm::vec3 m_Normal;
		float m_DotProduct;

		PlaneShape(const PlaneShape&) : iShape(eShapeType::Plane) { }
		PlaneShape& operator=(const PlaneShape&) { return *this; }
	};


	class AABBShape : public iShape
	{
	public:
		AABBShape(const glm::vec3& minimumValues, const glm::vec3& maximumValues);

		const glm::vec3& GetMin() { return min; }
		const glm::vec3& GetMax() { return max; }

		static AABBShape* Cast(iShape* shape);

	private:
		glm::vec3 min;		// Minimum values along each axis.
		glm::vec3 max;		// Maximum values along each axis.

		AABBShape(const AABBShape&) : iShape(eShapeType::AABB) { }
		AABBShape& operator=(const AABBShape&) { return *this; }
	};
}