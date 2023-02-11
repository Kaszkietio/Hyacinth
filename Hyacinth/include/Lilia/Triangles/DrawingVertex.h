#pragma once

#include <Lilia/Triangles/Vertex.h>
#include <WindowsNumerics.h>
using namespace Windows::Foundation::Numerics;

namespace Lilia
{
	struct DrawingVertexY
	{
		int32_t y{};
		float x{}, dx{};
		float z{}, dz{};

		inline void Update(
			int32_t y, 
			const glm::vec3& a,
			const glm::vec3& b
			)
		{
			this->y = y;
			float dy = (b.y - a.y);
			x = a.x;
			dx = (b.x - a.x) / dy;
			z = a.z;
			dz = (b.z - a.z) / dy;
		}

		inline void MakeStep()
		{
			y++;
			x += dx;
			z += dz;
		}
	};

	struct DrawingVertexX
	{
		int32_t x{};
		float z{}, dz{};

		inline void Update(
			int32_t x, 
			float x1, float x2,
			float z1, float z2
			)
		{
			this->x = x;
			float dx = (x2 - x1);
			z = z1;
			dz = (z2 - z1) / dx;
		}

		inline void MakeStep()
		{
			x++;
			z += dz;
		}
	};

	//struct DrawingVertexWorld
	//{
	//	float3 v{};
	//	float3 step{};

	//	inline void Update(
	//		float v1, float v2,
	//		const glm::vec4& a,
	//		const glm::vec4& b
	//	)
	//	{
	//		float t = v2 - v1;
	//		v = float3(a.x, a.y, a.z);
	//		step = (float3(b.x, b.y, b.z) - v) / t;
	//	}

	//	inline void Update(
	//		float v1, float v2,
	//		const float3& a,
	//		const float3& b
	//	)
	//	{
	//		float t = v2 - v1;
	//		v = a;
	//		step = (b - a) / t;
	//	}

	//	inline void MakeStep()
	//	{
	//		v += step;
	//	}
	//};
	struct DrawingVertexWorld
	{
		Vertex v{};
		glm::vec4 vertexStep{}, normalStep{};
		glm::vec2 texStep{};


		inline void Update(
			float v1, float v2,
			const Vertex& a,
			const Vertex& b
		)
		{
			float t = v2 - v1;
			v.vertex = a.vertex; v.normal = a.normal; v.texCoord = a.texCoord;
			vertexStep = (b.vertex - a.vertex) / t;
			normalStep = (b.normal - a.normal) / t;
			texStep = (b.texCoord - a.texCoord) / t;
		}

		inline void MakeStep()
		{
			v.vertex += vertexStep;
			v.normal += normalStep;
			v.texCoord += texStep;
		}
	};
}
