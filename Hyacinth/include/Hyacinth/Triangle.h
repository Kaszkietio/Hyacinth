#pragma once

#include <glm/glm.hpp>

#include <array>

namespace Hyacinth
{
#define FUN
#ifdef FUN
	struct Triangle
	{
		std::array<glm::vec4, 3> vertice{};
		std::array<glm::vec2, 3> texCoords{};
		std::array<glm::vec4, 3> normals{};
	};
#else
	struct Triangle
	{
		std::array<uint32_t, 3> vertexId{};
		std::array<uint32_t, 3> textureCoordId{};
		std::array<uint32_t, 3> normalId{};
	};
#endif
}
