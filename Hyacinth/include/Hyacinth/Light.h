#pragma once

#include <glm/glm.hpp>

namespace Hyacinth
{
	class Light
	{
	public:
		glm::vec4 Position{};
		glm::vec4 ForwardDirection{};
	};
}
