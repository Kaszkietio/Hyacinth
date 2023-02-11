#pragma once

#include <Lilia/Payloads.h>
#include <Lilia/Triangles/DrawingVertex.h>

namespace Lilia
{
	//struct FragmentShader
	//{
	//public:
	//	int32_t y{}, ymax{};
	//	int32_t x{}, xmax{};
	//	DrawingVertexY l{}, r{};
	//	DrawingVertexX current{};
	//	DrawingVertexWorld v1{}, v2{}, v{};

	//	inline void DrawLine(const FragmentData::Input& in);

	//	inline void DrawTriangle(const FragmentData::Input& in)
	//	{
	//		for (; y < ymax; y++, l.MakeStep(), r.MakeStep(), v1.MakeStep(), v2.MakeStep())
	//		{
	//			if (y < 0) continue;

	//			current.Update(l.x, l.x, r.x, l.z, r.z);
	//			x = current.x;
	//			xmax = std::min(int32_t(r.x) + 1, in.Fb.width);
	//			v.Update(l.x, r.x, v1.v, v2.v);

	//			DrawLine(in);
	//		}
	//	}

	//	inline void DrawFlatBottom(const FragmentData::Input& in, const ScreenTriangle& screen, const Triangle& t)
	//	{
	//		y = screen.v[0].vertex.y;
	//		ymax = std::min((int32_t)screen.v[2].vertex.y, in.Fb.height);
	//		l.Update(y, screen.v[0].vertex, screen.v[2].vertex);
	//		r.Update(y, screen.v[1].vertex, screen.v[2].vertex);
	//		v1.Update(
	//			screen.v[0].vertex.y, screen.v[2].vertex.y,
	//			t[0], t[2]
	//		);
	//		v2.Update(
	//			screen.v[1].vertex.y, screen.v[2].vertex.y,
	//			t[1], t[2]
	//		);

	//		DrawTriangle(in);
	//	}

	//	inline void DrawFlatTop(const FragmentData::Input& in, const ScreenTriangle& screen, const Triangle& t)
	//	{
	//		y = screen.v[0].vertex.y;
	//		ymax = std::min((int32_t)screen.v[2].vertex.y, in.Fb.height);
	//		l.Update(y, screen.v[0].vertex, screen.v[1].vertex);
	//		r.Update(y, screen.v[0].vertex, screen.v[2].vertex);
	//		v1.Update(
	//			screen.v[0].vertex.y, screen.v[1].vertex.y,
	//			t[0], t[1]
	//		);
	//		v2.Update(
	//			screen.v[0].vertex.y, screen.v[2].vertex.y,
	//			t[0], t[2]
	//		);

	//		DrawTriangle(in);
	//	}

	//	void Process(const FragmentData::Input& input);
	//	
	//	inline glm::vec3 Barycentric(
	//		const glm::vec3& p,
	//		const glm::vec3& a,
	//		const glm::vec3& b,
	//		const glm::vec3& c
	//	)
	//	{
	//		glm::vec3 result;
	//		glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	//		float d00 = glm::dot(v0, v0);
	//		float d01 = glm::dot(v0, v1);
	//		float d11 = glm::dot(v1, v1);
	//		float d20 = glm::dot(v2, v0);
	//		float d21 = glm::dot(v2, v1);
	//		float denom = d00 * d11 - d01 * d01;
	//		result.y = (d11 * d20 - d01 * d21) / denom;
	//		result.z = (d00 * d21 - d01 * d20) / denom;
	//		result.x = 1.0f - result.z - result.y;
	//		return result;
	//	}

	//	inline glm::vec4 ApplyBaricentric(
	//		const glm::vec4& a,
	//		const glm::vec4& b,
	//		const glm::vec4& c,
	//		const glm::vec3& ratio
	//	)
	//	{
	//		return glm::vec4(
	//			a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
	//			a.y * ratio.x + b.y * ratio.y + c.y * ratio.z,
	//			a.z * ratio.x + b.z * ratio.y + c.z * ratio.z,
	//			a.w * ratio.x + b.w * ratio.y + c.w * ratio.z
	//		);
	//	}

	//	inline glm::vec2 ApplyBaricentric(
	//		const glm::vec2& a,
	//		const glm::vec2& b,
	//		const glm::vec2& c,
	//		const glm::vec3& ratio
	//	)
	//	{
	//		return glm::vec2(
	//			a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
	//			a.y * ratio.x + b.y * ratio.y + c.y * ratio.z
	//		);
	//	}
	//};

	struct FragmentShader
	{
		struct Input
		{
			Framebuffer& fb;
			const std::vector<Triangle>& wholeWorld;
			std::vector<std::vector<Triangle>>& world;
			std::vector<std::vector<ScreenTriangle>>& screenTriangles;
			Texture& texture;
			ShadingModel& shadingModel;
			const std::vector<Light>& lights;
			const Camera& camera;
			const glm::vec3& cameraPosition;
		};

	public:
		int32_t inner{}, outer{};
		int32_t y{}, ymax{};
		int32_t x{}, xmax{};
		DrawingVertexY l{}, r{};
		DrawingVertexX current{};
		DrawingVertexWorld v1{}, v2{}, v{};

		inline void DrawLine(const Input& in);

		inline void DrawTriangle(const Input& in)
		{
			for (; y < ymax; y++, l.MakeStep(), r.MakeStep(), v1.MakeStep(), v2.MakeStep())
			{
				if (y < 0) continue;

				current.Update(l.x, l.x, r.x, l.z, r.z);
				x = current.x;
				xmax = std::min(int32_t(r.x) + 1, in.fb.width);
				v.Update(l.x, r.x, v1.v, v2.v);

				DrawLine(in);
			}
		}

		inline void DrawFlatBottom(const Input& in, const ScreenTriangle& screen, const Triangle& t)
		{
			y = screen.v[0].vertex.y;
			ymax = std::min((int32_t)screen.v[2].vertex.y, in.fb.height);
			l.Update(y, screen.v[0].vertex, screen.v[2].vertex);
			r.Update(y, screen.v[1].vertex, screen.v[2].vertex);
			v1.Update(
				screen.v[0].vertex.y, screen.v[2].vertex.y,
				t[0], t[2]
			);
			v2.Update(
				screen.v[1].vertex.y, screen.v[2].vertex.y,
				t[1], t[2]
			);

			DrawTriangle(in);
		}

		inline void DrawFlatTop(const Input& in, const ScreenTriangle& screen, const Triangle& t)
		{
			y = screen.v[0].vertex.y;
			ymax = std::min((int32_t)screen.v[2].vertex.y, in.fb.height);
			l.Update(y, screen.v[0].vertex, screen.v[1].vertex);
			r.Update(y, screen.v[0].vertex, screen.v[2].vertex);
			v1.Update(
				screen.v[0].vertex.y, screen.v[1].vertex.y,
				t[0], t[1]
			);
			v2.Update(
				screen.v[0].vertex.y, screen.v[2].vertex.y,
				t[0], t[2]
			);

			DrawTriangle(in);
		}

		void Process(const Input& input);

		//inline glm::vec3 Barycentric(
		//	const glm::vec3& p,
		//	const glm::vec3& a,
		//	const glm::vec3& b,
		//	const glm::vec3& c
		//)
		//{
		//	glm::vec3 result;
		//	glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
		//	float d00 = glm::dot(v0, v0);
		//	float d01 = glm::dot(v0, v1);
		//	float d11 = glm::dot(v1, v1);
		//	float d20 = glm::dot(v2, v0);
		//	float d21 = glm::dot(v2, v1);
		//	float denom = d00 * d11 - d01 * d01;
		//	result.y = (d11 * d20 - d01 * d21) / denom;
		//	result.z = (d00 * d21 - d01 * d20) / denom;
		//	result.x = 1.0f - result.z - result.y;
		//	return result;
		//}

		//inline glm::vec4 ApplyBaricentric(
		//	const glm::vec4& a,
		//	const glm::vec4& b,
		//	const glm::vec4& c,
		//	const glm::vec3& ratio
		//)
		//{
		//	return glm::vec4(
		//		a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
		//		a.y * ratio.x + b.y * ratio.y + c.y * ratio.z,
		//		a.z * ratio.x + b.z * ratio.y + c.z * ratio.z,
		//		a.w * ratio.x + b.w * ratio.y + c.w * ratio.z
		//	);
		//}

		//inline glm::vec2 ApplyBaricentric(
		//	const glm::vec2& a,
		//	const glm::vec2& b,
		//	const glm::vec2& c,
		//	const glm::vec3& ratio
		//)
		//{
		//	return glm::vec2(
		//		a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
		//		a.y * ratio.x + b.y * ratio.y + c.y * ratio.z
		//	);
		//}
	};
}
