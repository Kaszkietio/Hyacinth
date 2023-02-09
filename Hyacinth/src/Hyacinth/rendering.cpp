#include "Hyacinth/rendering.h"
#include "Hyacinth/utils.h"
#include <algorithm>
#include <vector>
#include <Hyacinth/Vertex.h>

inline void Hyacinth::DrawTriangleOutline(
	uint32_t data[],
	int32_t width,
	int32_t height,
	const Triangle& t,
	//const glm::vec4& a,
	//const glm::vec4& b,
	//const glm::vec4& c,
	uint32_t color
)
{
#if 0
	DrawLineBresenham(
		data, width, height,
		t.vertice[0].x, t.vertice[0].y,
		t.vertice[1].x, t.vertice[1].y,
		color
	);
	DrawLineBresenham(
		data, width, height,
		t.vertice[0].x, t.vertice[0].y,
		t.vertice[2].x, t.vertice[2].y,
		color
	);
	DrawLineBresenham(
		data, width, height,
		t.vertice[1].x, t.vertice[1].y,
		t.vertice[2].x, t.vertice[2].y,
		color
	);
	//DrawLineBresenham(
	//	data, width, height,
	//	a.x, a.y,
	//	b.x, b.y,
	//	color
	//);
	//DrawLineBresenham(
	//	data, width, height,
	//	a.x, a.y,
	//	c.x, c.y,
	//	color
	//);
	//DrawLineBresenham(
	//	data, width, height,
	//	b.x, b.y,
	//	c.x, c.y,
	//	color
	//);
#else

#endif
}

uint32_t Hyacinth::GetColor(
	std::vector<float>& zBuffer,
	int index,
	float u,
	float v,
	float z1,
	float z2,
	float q,
	const Texture& texture,
	uint32_t backgroundColor)
{
	float z = lerp(z1, z2, q);
	float cur = zBuffer[index];
	if (z > cur) return backgroundColor;

	zBuffer[index] = z;

	uint8_t r, g, b;
	int32_t x = texture.width * u, y = (texture.height - 1) * (1 - v);
	r = texture.data[y * texture.width * texture.channelCount + x * texture.channelCount];
	g = texture.data[y * texture.width * texture.channelCount + x * texture.channelCount + 1];
	b = texture.data[y * texture.width * texture.channelCount + x * texture.channelCount + 2];

	uint32_t color = 0xff000000
		| b << 16
		| g << 8
		| r;
	return color;
}

glm::vec4 Hyacinth::ConstShading(
	const glm::vec4& hitPoint,
	const glm::vec4& normal, 
	const glm::vec4& viewerPos,
	const std::vector<Light>& Lights)
{
	constexpr float ka = 0.2f;
	constexpr float kd = 0.6f;
	constexpr float ks = 0.2f;
	constexpr float n = 32.0f;

	std::vector<Light> Lights2;
	Lights2.push_back(Light
		{
			//.Position = glm::vec4(0.0f, 4.0f, 0.0f, 1.0f),
			//.ForwardDirection = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)
			.Position = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f),
			.ForwardDirection = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)
		});

	glm::vec4 intensity(ka);
	glm::vec4 v = glm::normalize(viewerPos - hitPoint);

	for (auto& light : Lights2)
	{
		glm::vec4 l = glm::normalize(light.Position - hitPoint);
		glm::vec4 r = glm::normalize(glm::reflect(l, normal));

		intensity += kd * glm::dot(normal, l) + ks * powf(glm::dot(v, r), n);
	}

	return intensity;
}


void Hyacinth::DrawTriangle(
	uint32_t data[],
	int32_t width,
	int32_t height,
	Triangle& triangleNDC,
	const Texture& texture,
	std::vector<float>& zBuffer,
	uint32_t& backColor
)
{
	Triangle triangle;
	triangle.vertice = triangleNDC.vertice;
	triangle.normals = triangleNDC.normals;
	triangle.texCoords = triangleNDC.texCoords;

	// to screen
	for (int i = 0; i < triangle.vertice.size(); i++)
	{
		triangle.vertice[i].x = (triangle.vertice[i].x + 1.0f) * width * 0.5f;
		triangle.vertice[i].y = (triangle.vertice[i].y + 1.0f) * height * 0.5f;
		triangle.vertice[i].z = (triangle.vertice[i].z + 1.0f) * 0.5f;
	}

	std::array<glm::vec4, 3>& vertice = triangle.vertice;

	// bubble sort prob quicker for triangle
	for (int i = 0; i < triangle.vertice.size(); i++)
	{
		for (int j = i+1; j < triangle.vertice.size(); j++)
		{
			if (!(vertice[i].y != vertice[j].y ? vertice[i].y < vertice[j].y : vertice[i].x < vertice[j].x))
			{
				std::swap(triangle.vertice[i], triangle.vertice[j]);
				std::swap(triangle.normals[i], triangle.normals[j]);
				std::swap(triangle.texCoords[i], triangle.texCoords[j]);
			}
		}
	}

	uint32_t altCol = 0xff000000 | ~(texture.data[0]);

	if (vertice[0].y == vertice[1].y)
	{
		DrawTriangleFlatBottom(
			data, width, height,
			triangle,
			texture,
			zBuffer,
			backColor
		);
		DrawTriangleOutline(
			data, width, height,
			triangle,
			altCol
		);
	}
	else if (vertice[2].y == vertice[1].y)
	{
		DrawTriangleFlatTop(
			data, width, height,
			triangle,
			texture,
			zBuffer,
			backColor
		);
		DrawTriangleOutline(
			data, width, height,
			triangle,
			altCol
		);
	}
	else
	{
		Triangle flatTop{};
		Triangle flatBottom{};

		glm::vec4 v4(0.0f);
		float dx20 = (vertice[2].x - vertice[0].x);
		float dy20 = (vertice[2].y - vertice[0].y);
		float dy10 = (vertice[1].y - vertice[0].y);
		float q = dy10 / dy20;

		v4 = lerp(vertice[0], vertice[2], q);
		glm::vec2 uv{};
		glm::vec2 uv2 = lerp(triangle.texCoords[0], triangle.texCoords[2], q);
		uv.x = lerp_w(triangle.texCoords[0].x, triangle.texCoords[2].x,
				triangle.vertice[0].w, triangle.vertice[2].w,
				q);
		uv.y = triangle.texCoords[1].y;
		uv = uv2;
		if (vertice[1].x < v4.x)
		{
			flatTop.vertice[0] = triangle.vertice[0];
			flatTop.vertice[1] = triangle.vertice[1];
			flatTop.vertice[2] = v4;

			flatTop.normals[0] = triangle.normals[0];
			flatTop.normals[1] = triangle.normals[1];
			flatTop.normals[2] = triangle.normals[1];

			flatTop.texCoords[0] = triangle.texCoords[0];
			flatTop.texCoords[1] = triangle.texCoords[1];
			flatTop.texCoords[2].y = triangle.texCoords[1].y;
			flatTop.texCoords[2] = uv;

			
			flatBottom.vertice[0] = triangle.vertice[1];
			flatBottom.vertice[1] = v4;
			flatBottom.vertice[2] = triangle.vertice[2];

			flatBottom.normals[0] = triangle.normals[1];
			flatBottom.normals[1] = triangle.normals[1];
			flatBottom.normals[2] = triangle.normals[2];

			flatBottom.texCoords[0] = triangle.texCoords[1];
			flatBottom.texCoords[1] = uv;
		}
		else
		{
			flatTop.vertice[0] = triangle.vertice[0];
			flatTop.vertice[1] = v4;
			flatTop.vertice[2] = triangle.vertice[1];

			flatTop.normals[0] = triangle.normals[0];
			flatTop.normals[1] = triangle.normals[1];
			flatTop.normals[2] = triangle.normals[1];

			flatTop.texCoords[0] = triangle.texCoords[0];
			flatTop.texCoords[1] = uv;
			flatTop.texCoords[2] = triangle.texCoords[1];

			
			flatBottom.vertice[0] = v4;
			flatBottom.vertice[1] = triangle.vertice[1];
			flatBottom.vertice[2] = triangle.vertice[2];

			flatBottom.normals[0] = triangle.normals[1];
			flatBottom.normals[1] = triangle.normals[1];
			flatBottom.normals[2] = triangle.normals[2];

			flatTop.texCoords[0] = uv;
			flatBottom.texCoords[1] = triangle.texCoords[1];
			flatBottom.texCoords[2] = triangle.texCoords[2];
		}

			DrawTriangleFlatTop(
				data, width, height,
				flatTop,
				texture,
				zBuffer,
				backColor
			);
			DrawTriangleFlatBottom(
				data, width, height,
				flatBottom,
				texture,
				zBuffer,
				backColor
			);
		DrawTriangleOutline(
			data, width, height,
			flatTop,
			altCol
		);
		DrawTriangleOutline(
			data, width, height,
			flatBottom,
			altCol
		);
	}
}

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
	const Triangle& t,
	//const glm::vec4& a,
	//const glm::vec4& b,
	//const glm::vec4& c,
	const Texture& texture,
	std::vector<float>& zBuffer,
	uint32_t& backColor
	)
{
	const glm::vec4& a = t.vertice[0], & b = t.vertice[1], & c = t.vertice[2];

	float z1 = a.z, z2 = a.z;
	glm::vec2 tex1 = t.texCoords[0], tex2 = t.texCoords[0];
	float q, q2;
	glm::vec2 uv;

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
		int32_t tmpx1 = std::max(x1, 0), tmpx2 = std::min(x2 + 1, width);
		float q = (float)(y - a.y) / (b.y - a.y);
		z1 = lerp(a.z, b.z, q);
		z2 = lerp(a.z, c.z, q);

		tex1 = lerp(t.texCoords[0], t.texCoords[1], q);
		tex2 = lerp(t.texCoords[0], t.texCoords[2], q);

		for (int x = tmpx1; x < tmpx2; x++)
		{
			if (q < 0.0f) break;
			q2 = float(x - tmpx1) / (tmpx2 - tmpx1);
			uv = lerp(tex1, tex2, q2);

			uint32_t col;
			col = GetColor(
				zBuffer,
				y * width + x,
				uv.x, uv.y,
				//0, 0,
				z1, z2, q2,
				texture,
				data[y * width + x]
			);
			// Continue if not recalculating pixel
			if(col == data[y * width + x]) continue;

			glm::vec3 objColor = glm::vec3(col % 256 / 256.0f, (col >> 8) % 256 / 256.0f, (col >> 16) % 256 / 256.0f);
			
			constexpr glm::vec3 lightColor(1.0f);
			constexpr float ambientStrength = 0.1f;
			glm::vec3 ambient = ambientStrength * lightColor;

			glm::vec3 result = ambient * objColor;

			glm::vec4 FragColor = glm::vec4(result, 1.0f);
			data[y * width + x] = to_rgba(FragColor);


			//data[y * width + x] = col;
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
	}
}

void Hyacinth::DrawTriangleFlatBottom(
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
	)
{
	const glm::vec4& a = t.vertice[0], & b = t.vertice[1], & c = t.vertice[2];
	float z1 = a.z, z2 = a.z;
	glm::vec2 tex1 = t.texCoords[0], tex2 = t.texCoords[0];
	float q, q2;
	glm::vec2 uv;

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
		int32_t tmpx1 = std::max(x1, 0), tmpx2 = std::min(x2 + 1, width);
		float q = (float)(y - a.y) / (c.y - a.y);
		z1 = lerp(a.z, c.z, q);
		z2 = lerp(b.z, c.z, q);

		//tex1 = lerp_w(t.texCoords[0], t.texCoords[2], t.vertice[0].w, t.vertice[2].w, q);
		//tex2 = lerp_w(t.texCoords[1], t.texCoords[2], t.vertice[1].w, t.vertice[2].w, q);
		tex1 = lerp(t.texCoords[0], t.texCoords[2], q);
		tex2 = lerp(t.texCoords[1], t.texCoords[2], q);

		for (int x = tmpx1; x < tmpx2; x++)
		{
			if (q < 0.0f) break;

			q2 = float(x - tmpx1) / (tmpx2 - tmpx1);
			uv = lerp(tex1, tex2, q2);
			float z = lerp(z1, z2, float(x - tmpx1) / (tmpx2 - tmpx1));

			uint32_t col;
			col = GetColor(
				zBuffer,
				y * width + x,
				uv.x, uv.y,
				//float(x - a.x) / (b.x - a.x), q,
				//q,float(x - a.x)/ (b.x - a.x),
				//1.0f, 1.0f,
				z1, z2, float(x - tmpx1) / (tmpx2 - tmpx1),
				texture,
				data[y * width + x]
			);
			// Continue if not recalculating pixel
			if(col == data[y * width + x]) continue;

			glm::vec3 objColor = glm::vec3(col % 256 / 256.0f, (col >> 8) % 256 / 256.0f, (col >> 16) % 256 / 256.0f);
			
			constexpr glm::vec3 lightColor(1.0f);
			constexpr float ambientStrength = 0.1f;
			constexpr glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 ambient = ambientStrength * lightColor;

			// diffuse
			glm::vec3 FragPos = glm::vec3(x, y, z);
			glm::vec3 norm = glm::normalize(glm::vec3(t.normals[0]));
			glm::vec3 lightDir = glm::normalize(lightPos - FragPos);
			auto tmp = glm::dot(norm, lightDir);
			float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
			glm::vec3 diffuse = diff * objColor;

			glm::vec3 result = (ambient + diffuse) * objColor;

			glm::vec4 FragColor = glm::vec4(result, 1.0f);
			data[y * width + x] = to_rgba(FragColor);
			//uint32_t col = 0xff000000 | texture.data[0];
			//data[y * width + x] = col;
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

					//glm::vec4 vC = lerp_w(A.vertex, B.vertex, A.vertex.w, B.vertex.w, distC);
					//glm::vec4 nC = lerp_w(A.normal, B.normal, A.vertex.w, B.vertex.w, distC);
					//glm::vec2 tC = lerp_w(A.texCoord, B.texCoord, A.vertex.w, B.vertex.w, distC);

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

					//glm::vec4 vC = lerp_w(A.vertex, B.vertex, A.vertex.w, B.vertex.w, distC);
					//glm::vec4 nC = lerp_w(A.normal, B.normal, A.vertex.w, B.vertex.w, distC);
					//glm::vec2 tC = lerp_w(A.texCoord, B.texCoord, A.vertex.w, B.vertex.w, distC);

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
	if (vertice.size() == 0) return {};
	std::vector<Triangle> result(std::max(vertice.size() - 2, (uint64_t)0));

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



