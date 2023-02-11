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
		int32_t h, w;

		inline uint32_t GetPixel(float u, float v) 
		{
			h = (int32_t)((height - 1) * (1 - v)) * width;
			w = int32_t(width * u);
			return ((uint32_t*)data)[h + w];
		}

		inline glm::vec3 GetPixelVector(float u, float v)
		{
			h = (int32_t)((height - 1) * (1 - v)) * width * channelCount;
			w = int32_t(width * u) * channelCount;
		return glm::vec3(
				(float)data[h + w] / 255.0f,
				(float)data[h + w + 1] / 255.0f,
				(float)data[h + w + 2] / 255.0f
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
