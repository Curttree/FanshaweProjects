#pragma once

#include <vector>
#include <extern/glm/vec3.hpp>

namespace gdp2022Physics
{
	class cSoftBodyDesc
	{
	public:
		cSoftBodyDesc(void) { }
		std::vector<glm::vec3> nodePositions;
		std::vector<unsigned int> triangulatedIndices;
	};
}