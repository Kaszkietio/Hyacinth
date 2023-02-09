#pragma once
#include <cstdint>
#include <vector>

namespace Lilia
{
	struct Framebuffer
	{
		uint32_t* data{ nullptr };
		int32_t width{}, height{};
		std::vector<float>& zBuffer;
	};
}
