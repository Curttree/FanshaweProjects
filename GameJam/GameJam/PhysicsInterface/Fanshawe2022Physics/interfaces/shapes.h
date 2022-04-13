#pragma once

#include "iShape.h"
#include <extern/glm/vec3.hpp>

namespace gdp2022Physics
{
	class BoxShape : public iShape
	{
	public:
		BoxShape(const glm::vec3& halfExtents);
		virtual ~BoxShape();

		const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

		static BoxShape* Cast(iShape* shape);

	private:
		glm::vec3 m_HalfExtents;

		BoxShape(const BoxShape&) : iShape(eShapeType::Box) { }
		BoxShape& operator=(const BoxShape&) { return *this; }
	};

	class CylinderShape : public iShape
	{
	public:
		CylinderShape(const glm::vec3& halfExtents);
		virtual ~CylinderShape();

		const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

		static CylinderShape* Cast(iShape* shape);

	private:
		glm::vec3 m_HalfExtents;

		CylinderShape(const CylinderShape&) : iShape(eShapeType::Cylinder) { }
		CylinderShape& operator=(const CylinderShape&) { return *this; }
	};
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
}