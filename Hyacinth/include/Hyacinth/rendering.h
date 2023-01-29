#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Hyacinth/Triangle.h>
#include <Hyacinth/Vertex.h>
#include <Hyacinth/Texture.h>

namespace Hyacinth
{
#ifndef FUN
	void DrawTriangle(
		uint32_t data[],
		int32_t width,
		int32_t height,
		std::array<glm::vec4, 3> triangle
		);
#else
	void DrawTriangle(
		uint32_t data[],
		int32_t width,
		int32_t height,
		const Triangle& triangle,
		const Texture& texture
		);
#endif
	void DrawLineBresenham(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		int32_t x, int32_t y,
		int32_t x2, int32_t y2,
		uint32_t color
	);

	void DrawTriangleFlatTop(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c,
		const Texture& texture
	);

	void DrawTriangleFlatBottom(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c,
		const Texture& texture
	);

	std::vector<Triangle> ClipTriangle(
		const Triangle& t
		);

	void SutherlandHodgman(
		std::vector<Vertex>& vertice);

	void DrawTriangleOutline(
		uint32_t data[],
		int32_t width,
		int32_t height,
		const glm::vec4& a,
		const glm::vec4& b,
		const glm::vec4& c,
		uint32_t color
	);
}
