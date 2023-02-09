#pragma once

#include <string>

namespace Lilia
{
	struct Texture
	{
		std::string Name{};

		int32_t width{};
		int32_t height{};
		int32_t channelCount{};
		uint8_t* data{ nullptr };

		constexpr uint32_t GetPixel(float u, float v)
		{
			return 0xff000000 
				| data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount + 2] << 16
				| data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount + 1] << 8
				| data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount];
		}

		constexpr glm::vec3 GetPixelVector(float u, float v)
		{
			return glm::vec3(
				(float)data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount] / 255.0f,
				(float)data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount + 1] / 255.0f,
				(float)data[(int32_t)((height - 1) * (1 - v)) * width * channelCount + int32_t(width * u) * channelCount + 2] / 255.0f
				);
		}
		static constexpr uint32_t ToRGBA(const glm::vec3& c)
		{
			return 0xff000000
				| uint8_t(c.b * 255.0f) << 16
				| uint8_t(c.g * 255.0f) << 8
				| uint8_t(c.r * 255.0f);
		}

	};
}
