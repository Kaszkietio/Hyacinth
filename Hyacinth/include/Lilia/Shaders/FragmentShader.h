#pragma once

#include <Lilia/Payloads.h>
#include <Lilia/Triangles/DrawingVertex.h>

namespace Lilia
{
	struct DrawingInput
	{
		Framebuffer& fb;
		Triangle wholeWorld;
		Triangle world;
		ScreenTriangle screen;
		ShadingModel& model;
		Texture& texture;
		const std::vector<Light>& lights;
		const glm::vec3& CameraPos;
	};

	static void SortByY(ScreenTriangle& st, Triangle& t)
	{
		for (int i = 0; i < st.v.size(); i++)
		{
			for (int j = i + 1; j < st.v.size(); j++)
			{
				if (!(st.v[i].vertex.y != st.v[j].vertex.y
					? st.v[i].vertex.y < st.v[j].vertex.y
					: st.v[i].vertex.x < st.v[j].vertex.x))
				{
					std::swap(st[i], st[j]);
					std::swap(t[i], t[j]);
				}
			}
		}
	}

	//struct DrawingVertex
	//{
	//	Vertex world;
	//	ScreenVertex screen;
	//	glm::vec4 vertexStep;
	//	glm::vec4 normalStep;
	//	glm::vec2 texStep;
	//	glm::vec3 screenStep;
	//	float wStep;

	//	DrawingVertex(
	//		const Vertex& v1, const ScreenVertex s1,
	//		const Vertex& v2, const ScreenVertex s2,
	//		float q
	//	)
	//	{
	//		world = v1; screen = s1;
	//		//vertexStep = (v2.vertex - v1.vertex) * q;
	//		//normalStep = (v2.normal - v1.normal) * q;
	//		//texStep = (v2.texCoord - v1.texCoord) * q;
	//		screenStep = (s2.vertex - s1.vertex) * q;
	//		//wStep = (s2.w - s1.w) * q;
	//	}

	//	inline void MakeStep()
	//	{
	//		//world.vertex += vertexStep;
	//		//world.normal += normalStep;
	//		//world.texCoord += texStep;
	//		screen.vertex += screenStep;
	//		//wStep += wStep;
	//		//world.vertex = world.vertex + vertexStep;
	//		//world.normal = world.normal + normalStep;
	//		//world.texCoord = world.texCoord + texStep;
	//		//screen.vertex = screen.vertex + screenStep;
	//		//screen.w = screen.w + wStep;
	//	}
	//};

	struct FragmentShader
	{
	public:
		int32_t y{}, ymax{};
		int32_t x{}, xmax{};
		DrawingVertexY l{}, r{};
		DrawingVertexX current{};
		DrawingVertexWorld v1{}, v2{}, v{};

		inline void DrawLine(const FragmentData::Input& in);

		inline void DrawTriangle(const FragmentData::Input& in)
		{
			for (; y < ymax; y++, l.MakeStep(), r.MakeStep(), v1.MakeStep(), v2.MakeStep())
			{
				if (y < 0) continue;

				current.Update(l.x, l.x, r.x, l.z, r.z);
				x = current.x;
				xmax = std::min(int32_t(r.x) + 1, in.Fb.width);
				v.Update(l.x, r.x, v1.v, v2.v);

				DrawLine(in);
			}
		}

		//inline void DrawFlatBottom(const DrawingInput& in)
		inline void DrawFlatBottom(const FragmentData::Input& in, const ScreenTriangle& screen, const Triangle& t)
		{
			y = screen.v[0].vertex.y;
			ymax = std::min((int32_t)screen.v[2].vertex.y, in.Fb.height);
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

		//inline void DrawFlatTop(const DrawingInput& in)
		inline void DrawFlatTop(const FragmentData::Input& in, const ScreenTriangle& screen, const Triangle& t)
		{
			y = screen.v[0].vertex.y;
			ymax = std::min((int32_t)screen.v[2].vertex.y, in.Fb.height);
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

		void Process(const FragmentData::Input& input);
		
		//inline glm::vec3 Barycentric(
		//	const glm::vec4& p,
		//	const glm::vec4& a,
		//	const glm::vec4& b,
		//	const glm::vec4& c
		//)
		//{
		//	glm::vec3 result;
		//	glm::vec4 v0 = b - a, v1 = c - a, v2 = p - a;
		//	float d00 = glm::dot(v0, v0);
		//	float d01 = glm::dot(v0, v1);
		//	float d11 = glm::dot(v1, v1);
		//	float d20 = glm::dot(v2, v0);
		//	float d21 = glm::dot(v2, v1);
		//	float denom = d00 * d11 - d01 * d01;
		//	result.x = (d11 * d20 - d01 * d21) / denom;
		//	result.z = (d00 * d21 - d01 * d20) / denom;
		//	result.y = 1.0f - result.x - result.z;
		//	return result;
		//}

		inline glm::vec3 Barycentric(
			const glm::vec3& p,
			const glm::vec3& a,
			const glm::vec3& b,
			const glm::vec3& c
		)
		{
			glm::vec3 result;
			glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			result.y = (d11 * d20 - d01 * d21) / denom;
			result.z = (d00 * d21 - d01 * d20) / denom;
			result.x = 1.0f - result.z - result.y;
			return result;
		}

		inline glm::vec4 ApplyBaricentric(
			const glm::vec4& a,
			const glm::vec4& b,
			const glm::vec4& c,
			const glm::vec3& ratio
		)
		{
			return glm::vec4(
				a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
				a.y * ratio.x + b.y * ratio.y + c.y * ratio.z,
				a.z * ratio.x + b.z * ratio.y + c.z * ratio.z,
				a.w * ratio.x + b.w * ratio.y + c.w * ratio.z
			);
		}

		inline glm::vec2 ApplyBaricentric(
			const glm::vec2& a,
			const glm::vec2& b,
			const glm::vec2& c,
			const glm::vec3& ratio
		)
		{
			return glm::vec2(
				a.x * ratio.x + b.x * ratio.y + c.x * ratio.z,
				a.y * ratio.x + b.y * ratio.y + c.y * ratio.z
			);
		}
		//static void DrawFlatBottom(DrawingInput& in)
		//{
		//	const ScreenVertex& a = in.screen.v[0], &b = in.screen.v[1], &c = in.screen.v[2];
		//	const Vertex& va = in.world[0], &vb = in.world[1], &vc = in.world[2];
		//	float invDY = 1.0f / (c.vertex.y - a.vertex.y);

		//	DrawingVertex l(va, a, vc, c, invDY);
		//	DrawingVertex r(vb, b, vc, c, invDY);

		//	int32_t y = a.vertex.y;
		//	int32_t ymax = std::min((int32_t)c.vertex.y, in.fb.height);

		//	DrawTriangle(in, l, r, y, ymax);
		//}

		//static void DrawFlatTop(DrawingInput& in)
		//{
		//	const ScreenVertex& a = in.screen.v[0], &b = in.screen.v[1], &c = in.screen.v[2];
		//	const Vertex& va = in.world[0], &vb = in.world[1], &vc = in.world[2];
		//	float invDY = 1.0f / (c.vertex.y - a.vertex.y);

		//	DrawingVertex l(va, a, vb, b, invDY);
		//	DrawingVertex r(va, a, vc, c, invDY);

		//	int32_t y = a.vertex.y;
		//	int32_t ymax = std::min((int32_t)c.vertex.y, in.fb.height);

		//	DrawTriangle(in, l, r, y, ymax);
		////}

		//static inline void DrawTriangle(DrawingInput& in, DrawingVertex& l, DrawingVertex& r, int32_t y, int32_t ymax)
		//{
		//	for (; y < ymax; y++)
		//	{
		//		if (y < 0) 
		//			continue;

		//		DrawLine(in, l, r, y);
		//		l.screen.vertex.x += l.screenStep.x;
		//		r.screen.vertex.x += r.screenStep.x;
		//		l.screen.vertex.z += l.screenStep.z;
		//		r.screen.vertex.z += r.screenStep.z;
		//	}
		//}

		//static void DrawLine(DrawingInput& in, DrawingVertex& l, DrawingVertex& r, int32_t y)
		//{
		//	int32_t xmin = std::max((int32_t)l.screen.vertex.x, 0);
		//	int32_t xmax = std::min((int32_t)r.screen.vertex.x + 1, in.fb.width);

		//	float invDX = 1.0f / (r.screen.vertex.x - l.screen.vertex.x);
		//	DrawingVertex current(l.world, l.screen, r.world, r.screen, invDX);
		//	ScreenVertex& s = current.screen;

		//	int32_t index = y * in.fb.width + xmin;
		//	for (int32_t x = l.screen.vertex.x; x < xmax; x++, current.MakeStep())
		//	{
		//		if (x < 0) 
		//			continue;
		//		// Check if sth closer was already drawn
		//		if (s.vertex.z < 0.0f || in.fb.zBuffer[y * in.fb.width + x] + FLT_EPSILON < s.vertex.z) continue;

		//		in.fb.zBuffer[y * in.fb.width + x] = s.vertex.z;
		//		auto dupa = in.model.GetColor(
		//			in.wholeWorld, 
		//			current.world, 
		//			in.lights, 
		//			in.texture,
		//			in.CameraPos
		//		);
		//		in.fb.data[y * in.fb.width + x] = Texture::ToRGBA(dupa);
		//		index++;
		//	}
		//}

	};
}
