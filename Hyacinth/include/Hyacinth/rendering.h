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
#include <Hyacinth/Light.h>

namespace Hyacinth
{
	void DrawTriangle(
		uint32_t data[],
		int32_t width,
		int32_t height,
		Triangle& triangleNDC,
		const Texture& texture,
		std::vector<float>& zBuffer,
		uint32_t& backColor
		);
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
		const Triangle& t,
		//const glm::vec4& a,
		//const glm::vec4& b,
		//const glm::vec4& c,
		const Texture& texture,
		std::vector<float>& zBuffer,
		uint32_t& backColor
	);

	void DrawTriangleFlatBottom(
		uint32_t data[],
		int32_t width, 
		int32_t height,
		const Triangle& t,
		//const glm::vec4& a,
		//const glm::vec4& b,
		//const glm::vec4& c,
		const Texture& texture,
		std::vector<float>& zBuffer,
		uint32_t& backColor
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
		const Triangle& t,
		//const glm::vec4& a,
		//const glm::vec4& b,
		//const glm::vec4& c,
		uint32_t color
	);

	inline uint32_t GetColor(
		std::vector<float>& zBuffer,
		int index,
		float u,
		float v,
		float z1,
		float z2,
		float q,
		const Texture& texture,
		uint32_t backgroundColor
	);

	glm::vec4 ConstShading(
		const glm::vec4& hitPoint,
		const glm::vec4& normal,
		const glm::vec4& viewerPos,
		const std::vector<Light>& Lights
	);

	template<class T>
	inline T lerp_w(T atr1, T atr2, float w1, float w2, float q)
	{
		return ((atr1 * (1 - q) / w1) + (atr2 * q / w2)) / ((1 - q) / w1 + q / w2);
	}

	template<class T>
	inline T lerp(T atr1, T atr2, float q)
	{
		return atr1 * (1 - q) + atr2 * q;
	}

	template<class T>
	inline T barimetric_w(T atr1, T atr2, T atr3, float b1, float b2, float b3, float w1, float w2, float w3)
	{
		return ((atr1 * b1 / w1) + (atr2 * b2 / w2) + (atr3 * b3 / w3)) / ((b1 / w1 + b2 / w2 + b3 / w3));
	}


}
