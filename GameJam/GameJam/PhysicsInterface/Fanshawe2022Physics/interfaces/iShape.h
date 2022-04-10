#pragma once

#include "eShapeType.h"

namespace gdp2022Physics
{
	class iShape
	{
	public:
		virtual ~iShape() {}

		eShapeType GetShapeType() const { return shapeType; }

	protected:
		iShape(eShapeType shapeType)
			: shapeType(shapeType) { }

	private:
		eShapeType shapeType;

		iShape(const iShape&) {}
		iShape& operator=(const iShape&) { return *this; }
	};
}
