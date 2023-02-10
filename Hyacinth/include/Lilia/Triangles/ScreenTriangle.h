#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <Lilia/Framebuffer.h>
#include <Lilia/Texture.h>
#include <Lilia/Triangles/Vertex.h>

namespace Lilia
{
	struct ScreenVertex
	{
		glm::vec3 vertex{};
		float w{};

		static inline ScreenVertex lerp(const ScreenVertex& a, const ScreenVertex& b, float q)
		{
			ScreenVertex result;
			result.vertex = glm::lerp(a.vertex, b.vertex, glm::vec3(q));
			result.w = glm::lerp(a.w, b.w, q);
			return result;
		}
	};

	struct ScreenTriangle
	{
		std::array<ScreenVertex, 3> v{};
		//Texture& texture;

		constexpr ScreenVertex& operator[](int index)
		{
			return v[index];
		}

		void SortByY()
		{
			// Sort by y
			for (int i = 0; i < v.size(); i++)
			{
				for (int j = i + 1; j < v.size(); j++)
				{
					if (!(v[i].vertex.y != v[j].vertex.y
						? v[i].vertex.y < v[j].vertex.y
						: v[i].vertex.x < v[j].vertex.x))
					{
						std::swap(v[i].vertex, v[j].vertex);
					}
				}
			}
		}

		constexpr uint64_t size() { return v.size(); }

		struct DrawLinePayload
		{
			int y;
			int x1, x2;
			float z1, z2;
			glm::vec2 uv1, uv2;

			void Update(int y, int x1, int x2, float z1, float z2, const glm::vec2& uv1, const glm::vec2& uv2)
			{
				this->y = y;
				this->x1 = x1;
				this->x2 = x2;
				this->z1 = z1;
				this->z2 = z2;
				this->uv1 = uv1;
				this->uv2 = uv2;
			}
		};

		inline void DrawLine(Framebuffer& fb, DrawLinePayload& p)
		{
			constexpr uint32_t color = 0xff0000ff;

			int xmin = std::max(p.x1, 0);
			int xmax = std::min(p.x2 + 1, fb.width);

			float z = p.z1;
			float invDX = (p.x2 - p.x1);
			float dz = float(p.z2 - p.z1) / invDX;

			float quv = 0.0f;
			float dquv = 1 / invDX;

			glm::vec2 uv;

			for (int x = xmin; x < xmax; x++, z += dz, quv += dquv)
			{
				if (z < 0.0f || fb.zBuffer[p.y * fb.width + x] + FLT_EPSILON < z) continue;

				uv = glm::lerp(p.uv1, p.uv2, glm::vec2(quv));

				fb.zBuffer[p.y * fb.width + x] = z;
				//uint32_t newColor = texture.GetPixel(uv.x, uv.y);
				//fb.data[p.y * fb.width + x] = newColor;
				fb.data[p.y * fb.width + x] = 0xff0000ff;
			}
#define DrawOutline 0
#if DrawOutline
			if (p.y * fb.width + xmin >= 0 
				&& p.y * fb.width + xmin < fb.width * fb.height)
					fb.data[p.y * fb.width + xmin] = 0xff000000 | ~color;
			if (p.y * fb.width + xmax - 1 >= 0 
				|| p.y * fb.width + xmax - 1 < fb.width * fb.height) 
				fb.data[p.y * fb.width + xmax - 1] = 0xff000000 | ~color;
#endif
		}

		inline void DrawFlatBottom(Framebuffer& fb)
		{
			DrawLinePayload payload;

			const ScreenVertex& a = v[0], & b = v[1], & c = v[2];
			int32_t y;

			float x1 = a.vertex.x, x2 = b.vertex.x;
			float dy = c.vertex.y - a.vertex.y;
			float dx1 = (c.vertex.x - a.vertex.x) / dy,
				dx2 = (c.vertex.x - b.vertex.x) / dy;

			float z1 = a.vertex.z, z2 = b.vertex.z;
			float dz1 = (c.vertex.z - a.vertex.z) / dy;
			float dz2 = (c.vertex.z - b.vertex.z) / dy;

			//glm::vec2 uv1 = a.texCoord, uv2 = b.texCoord;
			//glm::vec2 uv3 = c.texCoord;

			float quv = 0.0f;
			float dquv = 1 / dy;

			int32_t ymax = std::min((int32_t)c.vertex.y, fb.height);

			for (y = a.vertex.y; 
				y < ymax;
				y++, 
				x1 += dx1, x2 += dx2, 
				z1 += dz1, z2 += dz2,
				quv += dquv)
			{
				if (y < 0) continue;
				payload.Update(
					y,
					x1, x2,
					z1, z2,
					//glm::lerp(uv1, uv3, glm::vec2(quv)), glm::lerp(uv2, uv3, glm::vec2(quv))
					glm::vec2(0.0f), glm::vec2(0.0f)
				);
				DrawLine(fb, payload);
			}
		}

		inline void DrawFlatTop(Framebuffer& fb)
		{
			DrawLinePayload payload;

			const ScreenVertex& a = v[0], & b = v[1], & c = v[2];
			int32_t y;

			float x1 = a.vertex.x, x2 = a.vertex.x;
			float dy = c.vertex.y - a.vertex.y;
			float dx1 = (b.vertex.x - a.vertex.x) / dy,
				dx2 = (c.vertex.x - a.vertex.x) / dy;

			float z1 = a.vertex.z, z2 = a.vertex.z;
			float dz1 = (b.vertex.z - a.vertex.z) / dy;
			float dz2 = (c.vertex.z - a.vertex.z) / dy;

			//glm::vec2 uv1 = a.texCoord, uv2 = b.texCoord;
			//glm::vec2 uv3 = c.texCoord;

			float quv = 0.0f;
			float dquv = 1 / dy;

			int32_t ymax = std::min((int32_t)c.vertex.y, fb.height);

			for (y = a.vertex.y; 
				y < ymax;
				y++, 
				x1 += dx1, x2 += dx2, 
				z1 += dz1, z2 += dz2,
				quv += dquv
				)
			{
				if (y < 0) continue;
				payload.Update(
					y,
					x1, x2,
					z1, z2,
					//glm::lerp(uv1, uv2, glm::vec2(quv)), glm::lerp(uv1, uv3, glm::vec2(quv))
					glm::vec2(0.0f), glm::vec2(0.0f)
				);
				DrawLine(fb, payload);
			}
		}
	};

}
