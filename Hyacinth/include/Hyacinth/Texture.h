#pragma once
#include <string>

namespace Hyacinth
{
	struct Texture
	{
		std::string Name{};

		uint32_t width{};
		uint32_t height{};
		uint32_t channelCount{};
		uint8_t* data{ nullptr };
	};
}
