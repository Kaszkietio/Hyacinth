#pragma once

#include <glm/glm.hpp>

namespace Lilia
{
	class Light
	{
	public:
		glm::vec4 Position{};
		glm::vec4 ForwardDirection{};
	};
}
