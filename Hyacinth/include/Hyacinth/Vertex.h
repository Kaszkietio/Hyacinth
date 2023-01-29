#pragma once
#include <glm/glm.hpp>

namespace Hyacinth
{
	struct Vertex
	{
		glm::vec4 vertex{};
		glm::vec4 normal{};
		glm::vec2 texCoord{};
	};
}
