#pragma once
#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <cstdint>

namespace Hyacinth
{
	constexpr uint32_t to_rgba(const glm::vec3& v)
	{
		return 0xff000000
			| (int)(v.z * 255.0f) << 16
			| (int)(v.y * 255.0f) << 8
			| (int)(v.x * 255.0f);
	}

	constexpr uint32_t to_rgba(const float& r, const float& g, const float& b)
	{
		return 0xff000000
			| (int)(b * 255.0f) << 16
			| (int)(g * 255.0f) << 8
			| (int)(r * 255.0f);
	}
}
