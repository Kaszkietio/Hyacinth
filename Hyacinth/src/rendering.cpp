#include "Hyacinth/rendering.h"
#include "Hyacinth/utils.h"
#include <algorithm>
#include <vector>
#include <Hyacinth/Vertex.h>

#ifndef FUN
void Hyacinth::DrawTriangle(
	uint32_t data[], 
	int32_t width, 
	int32_t height, 
	std::array<glm::vec4, 3> triangle
	)
{
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
#else

void Hyacinth::DrawTriangleOutline(
	uint32_t data[],
	int32_t width,
	int32_t height,
	const glm::vec4& a,
	const glm::vec4& b,
	const glm::vec4& c,
	uint32_t color
)
{
#if 1
	DrawLineBresenham(
		data, width, height,
		a.x, a.y,
		b.x, b.y,
		color
	);
	DrawLineBresenham(
		data, width, height,
		a.x, a.y,
		c.x, c.y,
		color
	);
	DrawLineBresenham(
		data, width, height,
		c.x, c.y,
		b.x, b.y,
		color
	);
#else

#endif
}


void Hyacinth::DrawTriangle(
	uint32_t data[],
	int32_t width,
	int32_t height,
	const Triangle& triangle,
	const Texture& texture
)
{
	std::array<glm::vec4, 3> vertice = triangle.vertice;
	std::sort(vertice.begin(), vertice.end(), [](const glm::vec4& a, const glm::vec4& b) { return a.y != b.y ? a.y < b.y : a.x < b.x; });
	uint32_t altCol = 0xff000000 | ~(texture.data[0]);

	if (vertice[0].y == vertice[1].y)
	{
		DrawTriangleFlatBottom(
			data, width, height,
			vertice[0], vertice[1], vertice[2],
			texture
		);
		DrawTriangleOutline(
			data, width, height,
			vertice[0], vertice[1], vertice[2],
			altCol
		);
	}
	else if (vertice[2].y == vertice[1].y)
	{
		DrawTriangleFlatTop(
			data, width, height,
			vertice[0], vertice[1], vertice[2],
			texture
		);
		DrawTriangleOutline(
			data, width, height,
			vertice[0], vertice[1], vertice[2],
			altCol
		);
	}
	else
	{
		glm::vec4 v4(0.0f);

		v4.x = vertice[0].x + ((vertice[1].y - vertice[0].y) / (vertice[2].y - vertice[0].y)) * (vertice[2].x - vertice[0].x);
		v4.y = vertice[1].y;
		if (vertice[1].x < v4.x)
		{
			DrawTriangleFlatTop(
				data, width, height,
				vertice[0], vertice[1], v4,
				texture
			);
			DrawTriangleFlatBottom(
				data, width, height,
				vertice[1], v4, vertice[2],
				texture
			);
		DrawTriangleOutline(
			data, width, height,
			vertice[0], vertice[1], v4,
			altCol
		);
		DrawTriangleOutline(
			data, width, height,
			vertice[1], v4, vertice[2],
			altCol
		);
		}
		else
		{
			DrawTriangleFlatTop(
				data, width, height,
				vertice[0], v4, vertice[1],
				texture
			);
			DrawTriangleFlatBottom(
				data, width, height,
				v4, vertice[1], vertice[2],
				texture
			);
		DrawTriangleOutline(
			data, width, height,
			vertice[0], v4, vertice[1],
			altCol
		);
		DrawTriangleOutline(
			data, width, height,
			v4, vertice[1],  vertice[2],
			altCol
		);
		}
	}
}
#endif

void Hyacinth::DrawLineBresenham(
	uint32_t data[],
	int32_t width,
	int32_t height,
	int32_t x, int32_t y,
	int32_t x2, int32_t y2,
	uint32_t color
)
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
		if(x >= 0 && x < width && y >= 0 && y < height)
			data[y * width + x] = color;
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
	const glm::vec4& c,
	const Texture& texture
	)
{
	int32_t dx1 = 0, dx2 = 0;
	int32_t w1 = b.x - a.x, w2 = c.x - a.x;
	int32_t h = b.y - a.y;
	if (h == 0) return;

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
			pk1 += h;
		}
		pk1 -= w1;

		while (pk2 < 0)
		{
			x2 += dx2;
			pk2 += h;
		}
		pk2 -= w2;

		if (y < 0) continue;
		for (int x = std::max(x1, 0); x < std::min(x2 + 1, width); x++)
		{
			data[y * width + x] = 0xff000000 | texture.data[0];
			//glm::vec2 p = { x, y };
			//glm::vec2 v0 = b - a, v1 = c - a, v2 = p - glm::vec2(a);
			//float d00 = glm::dot(v0, v0);
			//float d01 = glm::dot(v0, v1);
			//float d11 = glm::dot(v1, v1);
			//float d20 = glm::dot(v2, v0);
			//float d21 = glm::dot(v2, v1);
			//float denom = d00 * d11 - d01 * d01;
			//float v = (d11 * d20 - d01 * d21) / denom;
			//float w = (d00 * d21 - d01 * d20) / denom;
			//float u = 1.0f - v - w;
			//data[y * width + x] = to_rgba(glm::vec3(u, v, w));
		}
	}
}

void Hyacinth::DrawTriangleFlatBottom(
	uint32_t data[], 
	int32_t width, 
	int32_t height, 
	const glm::vec4& a, 
	const glm::vec4& b, 
	const glm::vec4& c,
	const Texture& texture
	)
{
	int32_t dx1 = 0, dx2 = 0;
	int32_t w1 = c.x - a.x, w2 = c.x - b.x;
	int32_t h = c.y - a.y;
	if (h == 0) return;

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
			pk1 += h;
		}
		pk1 -= w1;

		while (pk2 < 0)
		{
			x2 += dx2;
			pk2 += h;
		}
		pk2 -= w2;

		if (y < 0) continue;
		for (int x = std::max(x1, 0); x < std::min(x2 + 1, width); x++)
		{
			data[y * width + x] = 0xff000000 | texture.data[0];
			//glm::vec2 p = { x, y };
			//glm::vec2 v0 = b - a, v1 = c - a, v2 = p - glm::vec2(a);
			//float d00 = glm::dot(v0, v0);
			//float d01 = glm::dot(v0, v1);
			//float d11 = glm::dot(v1, v1);
			//float d20 = glm::dot(v2, v0);
			//float d21 = glm::dot(v2, v1);
			//float denom = d00 * d11 - d01 * d01;
			//float v = (d11 * d20 - d01 * d21) / denom;
			//float w = (d00 * d21 - d01 * d20) / denom;
			//float u = 1.0f - v - w;
			//data[y * width + x] = to_rgba(glm::vec3(u, v, w));

		}
	}
}

inline void Hyacinth::SutherlandHodgman(std::vector<Vertex>& vertice)
{
	constexpr auto dist = [](const glm::vec4& v, int i) -> float 
	{
		float tmp = (i % 2 == 0 ? -1 : 1) * v[i / 2];
		float res = v.w + tmp;
		return res;
	};

	std::vector<Vertex> tmp;

	// Create clipped polygon
	for (int border = 0; border < 6; border++)
	{
		tmp.clear();
		int index = vertice.size() - 1;
		for (int i = 0; i < vertice.size(); i++, index++)
		{
			auto& A = vertice[index % vertice.size()];
			auto& B = vertice[(index + 1) % vertice.size()];
			float distA = dist(A.vertex, border);
			float distB = dist(B.vertex, border);

			float signA = glm::sign(distA), signB = glm::sign(distB);

			// Start outside
			if (signA < 0)
			{
				// End outside
				if (signB < 0)
				{
					// Nothing
				}
				// End inside
				else
				{
					float distC = distA / (distA - distB);
					glm::vec4 vC = A.vertex * (1 - distC) + B.vertex * distC;
					glm::vec4 nC = A.normal * (1 - distC) + B.normal * distC;
					glm::vec2 tC = A.texCoord * (1 - distC) + B.texCoord * distC;
					tmp.push_back(Vertex{.vertex = vC, .normal = nC, .texCoord = tC});
					tmp.push_back(B);
				}
			}
			// Start inside
			else
			{
				// End outside
				if (signB < 0)
				{
					float distC = distA / (distA - distB);
					glm::vec4 vC = A.vertex * (1 - distC) + B.vertex * distC;
					glm::vec4 nC = A.normal * (1 - distC) + B.normal * distC;
					glm::vec2 tC = A.texCoord * (1 - distC) + B.texCoord * distC;
					tmp.push_back(Vertex{.vertex = vC, .normal = nC, .texCoord = tC});
				}
				// End inside
				else
				{
					tmp.push_back(B);
				}
			}
		}

		// copy tmp to vertice
		vertice = tmp;
	}
}

std::vector<Hyacinth::Triangle> Hyacinth::ClipTriangle(
	const Triangle& t
)
{
	std::vector<Vertex> vertice(t.vertice.size());

	for (int i = 0; i < t.vertice.size(); i++)
	{
		vertice[i].vertex = t.vertice[i];
		vertice[i].normal = t.normals[i];
		vertice[i].texCoord = t.texCoords[i];
	}

	// Apply Sutherland-Hodgman
	SutherlandHodgman(vertice);
	// We have current polygon, gotta create triangles out of it
	std::vector<Triangle> result(vertice.size() - 2);

	for (int i = 0; i < vertice.size() - 2; i++)
	{
		Triangle tmp{};
		for (int j = 0; j < tmp.vertice.size(); j++)
		{
			int index = j != 0 ? i + j : 0;
			tmp.vertice[j] = vertice[index].vertex;
			tmp.normals[j] = vertice[index].normal;
			tmp.texCoords[j] = vertice[index].texCoord;
		}
		result[i] = tmp;
	}

	return result;
}



