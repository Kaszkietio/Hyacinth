#pragma once
#include <cstdint>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hyacinth
{
	void DrawTriangle(
		uint32_t data[],
		int32_t width,
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c
		);

	void DrawLineBresenham(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		int32_t x, int32_t y,
		int32_t x2, int32_t y2
	);

	void DrawTriangleFlatTop(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c
	);

	void DrawTriangleFlatBottom(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c
	);
}
