#include "Hyacinth/rendering.h"
#include "Hyacinth/utils.h"
#include <algorithm>
#include <vector>

void Hyacinth::DrawTriangle(
	uint32_t data[], 
	int32_t width, 
	int32_t height, 
	const glm::vec4& a, 
	const glm::vec4& b,
	const glm::vec4& c
	)
{
	std::vector<glm::vec4> triangle(3);
	triangle[0] = std::move(glm::vec4((int)a.x, (int)a.y, a.z, 1.0f));
	triangle[1] = std::move(glm::vec4((int)b.x, (int)b.y, b.z, 1.0f));
	triangle[2] = std::move(glm::vec4((int)c.x, (int)c.y, c.z, 1.0f));
	std::sort(triangle.begin(), triangle.end(), [](const glm::vec4& a, const glm::vec4& b) { return a.y != b.y ? a.y < b.y : a.x < b.x; });

	if (triangle[0].y == triangle[1].y)
	{
		DrawTriangleFlatBottom(
			data, width, height,
			triangle[0], triangle[1], triangle[2]
		);
	}
	else if (triangle[2].y == triangle[1].y)
	{
		DrawTriangleFlatTop(
			data, width, height,
			triangle[0], triangle[1], triangle[2]
		);
	}
	else
	{
		glm::vec4 v4(0.0f);

		v4.x = triangle[0].x + ((triangle[1].y - triangle[0].y) / (triangle[2].y - triangle[0].y)) * (triangle[2].x - triangle[0].x);
		v4.y = triangle[1].y;
		if (triangle[1].x < v4.x)
		{
			DrawTriangleFlatTop(
				data, width, height,
				triangle[0], triangle[1], v4
			);
			DrawTriangleFlatBottom(
				data, width, height,
				triangle[1], v4, triangle[2]
			);
		}
		else
		{
			DrawTriangleFlatTop(
				data, width, height,
				triangle[0], v4, triangle[1]
			);
			DrawTriangleFlatBottom(
				data, width, height,
				v4, triangle[1], triangle[2]
			);
		}
	}
}

void Hyacinth::DrawLineBresenham(uint32_t data[], int32_t width, int32_t height, int32_t x, int32_t y, int32_t x2, int32_t y2)
{
	int w = x2 - x;
	int h = y2 - y;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	int longest = std::abs(w);
	int shortest = std::abs(h);
	if (!(longest > shortest)) {
		longest = std::abs(h);
		shortest = std::abs(w);
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	int numerator = longest >> 1;
	for (int i = 0; i <= longest; i++) {
		//putpixel(x, y, color);
		if(x >= 0 && x < width && y >= 0 && y < height)
			data[y * width + x] = 0xffffffff;
		numerator += shortest;
		if (!(numerator < longest)) {
			numerator -= longest;
			x += dx1;
			y += dy1;
		}
		else {
			x += dx2;
			y += dy2;
		}
	}
}

void Hyacinth::DrawTriangleFlatTop(
	uint32_t data[], 
	int32_t width, 
	int32_t height, 
	const glm::vec4& a, 
	const glm::vec4& b, 
	const glm::vec4& c
	)
{
	constexpr uint32_t fillColor = 0xff3399ff;//to_rgba(glm::vec3(1.0f, ));

	int32_t w1 = b.x - a.x, w2 = c.x - a.x;
	int32_t h1 = b.y - a.y, h2 = h1;
	int32_t dx1 = 0, dx2 = 0;

	if (w1 > 0) dx1 = 1;
	else if (w1 < 0) dx1 = -1;

	if (w2 > 0) dx2 = 1;
	else if (w2 < 0) dx2 = -1;

	w1 *= dx1; w2 *= dx2;

	int32_t pk1 = w1 >> 1;
	int32_t pk2 = w2 >> 1;

	for (int32_t x1 = a.x, x2 = a.x, y = a.y; y <= b.y && y < height; y++)
	{
		while (pk1 < 0)
		{
			x1 += dx1;
			pk1 += h1;
		}
		pk1 -= w1;

		while (pk2 < 0)
		{
			x2 += dx2;
			pk2 += h2;
		}
		pk2 -= w2;

		if (y < 0) continue;
		for (int x = std::max(x1, 0); x < std::min(x2 + 1, width); x++)
		{
			//data[y * width + x] = fillColor;
			glm::vec2 p = { x, y };
			glm::vec2 v0 = b - a, v1 = c - a, v2 = p - glm::vec2(a);
			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;
			data[y * width + x] = to_rgba(glm::vec3(u, v, w));
		}
	}
}

void Hyacinth::DrawTriangleFlatBottom(
	uint32_t data[], 
	int32_t width, 
	int32_t height, 
	const glm::vec4& a, 
	const glm::vec4& b, 
	const glm::vec4& c
	)
{
	constexpr uint32_t fillColor = to_rgba(glm::vec3(1.0f));

	int32_t w1 = c.x - a.x, w2 = c.x - b.x;
	int32_t h1 = c.y - a.y, h2 = h1;
	int32_t dx1 = 0, dx2 = 0;

	if (w1 > 0) dx1 = 1;
	else if (w1 < 0) dx1 = -1;

	if (w2 > 0) dx2 = 1;
	else if (w2 < 0) dx2 = -1;

	w1 *= dx1; w2 *= dx2;

	int32_t pk1 = w1 >> 1;
	int32_t pk2 = w2 >> 1;

	for (int32_t x1 = a.x, x2 = b.x, y = a.y; y <= c.y && y < height; y++)
	{
		while (pk1 < 0)
		{
			x1 += dx1;
			pk1 += h1;
		}
		pk1 -= w1;

		while (pk2 < 0)
		{
			x2 += dx2;
			pk2 += h2;
		}
		pk2 -= w2;

		if (y < 0) continue;
		for (int x = std::max(x1, 0); x < std::min(x2 + 1, width); x++)
		{
			//data[y * width + x] = fillColor;
			glm::vec2 p = { x, y };
			glm::vec2 v0 = b - a, v1 = c - a, v2 = p - glm::vec2(a);
			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;
			data[y * width + x] = to_rgba(glm::vec3(u, v, w));

		}
	}
}


