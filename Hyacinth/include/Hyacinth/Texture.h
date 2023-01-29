#pragma once
#include <string>

namespace Hyacinth
{
	struct Texture
	{
		std::string Name{};

		int32_t width{};
		int32_t height{};
		int32_t channelCount{};
		uint8_t* data{ nullptr };
	};
}
