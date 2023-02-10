#pragma once

#include <Lilia/Payloads.h>

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

	struct DrawingVertex
	{
		Vertex world;
		ScreenVertex screen;
		glm::vec4 vertexStep;
		glm::vec4 normalStep;
		glm::vec2 texStep;
		glm::vec3 screenStep;
		float wStep;

		DrawingVertex(
			const Vertex& v1, const ScreenVertex s1,
			const Vertex& v2, const ScreenVertex s2,
			float q
		)
		{
			world = v1; screen = s1;
			vertexStep = (v2.vertex - v1.vertex) * q;
			normalStep = (v2.normal - v1.normal) * q;
			texStep = (v2.texCoord - v1.texCoord) * q;
			screenStep = (s2.vertex - s1.vertex) * q;
			wStep = (s2.w - s1.w) * q;
		}

		inline void MakeStep()
		{
			world.vertex += vertexStep;
			world.normal += normalStep;
			world.texCoord += texStep;
			screen.vertex += screenStep;
			//wStep += wStep;
			//world.vertex = world.vertex + vertexStep;
			//world.normal = world.normal + normalStep;
			//world.texCoord = world.texCoord + texStep;
			//screen.vertex = screen.vertex + screenStep;
			//screen.w = screen.w + wStep;
		}
	};

	class FragmentShader
	{
	public:
		static void Process(const FragmentData::Input& input)
		{
			Triangle world = input.World;
			ScreenTriangle screen = input.WholeScreenTriangle;
			input.Model.Update(world, input.Lights, input.CameraPos);
			for (auto& clipped : input.ScreenTriangles)
			{
				DrawingInput drawingInput{
					.fb = input.Fb,
					.wholeWorld = world,
					.world = world,
					.screen = clipped,
					.model = input.Model,
					.texture = input.Tex,
					.lights = input.Lights,
					.CameraPos = input.CameraPos
				};

				// sort by y axis
				SortByY(screen, world);

				// Its simply Flat bottom triangle
				if (screen[0].vertex.y == screen[1].vertex.y)
					DrawFlatBottom(drawingInput);
				// its simply flat top triangle
				else if (screen[1].vertex.y == screen[2].vertex.y)
					DrawFlatTop(drawingInput);
				// more complex case => divide into flat top + flat bottom triangles
				else
				{
					float q = (screen[1].vertex.y - screen[0].vertex.y) / (screen[2].vertex.y - screen[0].vertex.y);
					ScreenVertex m = ScreenVertex::lerp(screen[0], screen[2], q);
					Vertex mv = Vertex::lerp(world[0], world[2], q);
					m.vertex.y = screen[1].vertex.y;

					ScreenTriangle flatTop = screen, flatBot = screen;
					flatTop[2] = m;
					flatBot[0] = m;
					Triangle flatTopT = world, flatBotT = world;
					flatTopT[2] = mv;
					flatBotT[0] = mv;

					SortByY(flatTop, flatTopT);
					SortByY(flatBot, flatBotT);

					drawingInput.screen = flatTop;
					drawingInput.world = flatTopT;
					DrawFlatTop(drawingInput);

					drawingInput.screen = flatBot;
					drawingInput.world = flatBotT;
					DrawFlatBottom(drawingInput);
				}
			}
		}

		static void DrawFlatBottom(DrawingInput& in)
		{
			const ScreenVertex& a = in.screen.v[0], &b = in.screen.v[1], &c = in.screen.v[2];
			const Vertex& va = in.world[0], &vb = in.world[1], &vc = in.world[2];
			float invDY = 1.0f / (c.vertex.y - a.vertex.y);

			DrawingVertex l(va, a, vc, c, invDY);
			DrawingVertex r(vb, b, vc, c, invDY);

			int32_t y = a.vertex.y;
			int32_t ymax = std::min((int32_t)c.vertex.y, in.fb.height);

			DrawTriangle(in, l, r, y, ymax);
		}

		static void DrawFlatTop(DrawingInput& in)
		{
			const ScreenVertex& a = in.screen.v[0], &b = in.screen.v[1], &c = in.screen.v[2];
			const Vertex& va = in.world[0], &vb = in.world[1], &vc = in.world[2];
			float invDY = 1.0f / (c.vertex.y - a.vertex.y);

			DrawingVertex l(va, a, vb, b, invDY);
			DrawingVertex r(va, a, vc, c, invDY);

			int32_t y = a.vertex.y;
			int32_t ymax = std::min((int32_t)c.vertex.y, in.fb.height);

			DrawTriangle(in, l, r, y, ymax);
		}

		static inline void DrawTriangle(DrawingInput& in, DrawingVertex& l, DrawingVertex& r, int32_t y, int32_t ymax)
		{
			for (; y < ymax; y++, l.MakeStep(), r.MakeStep())
			{
				if (y < 0) 
					continue;

				DrawLine(in, l, r, y);
			}
		}

		static void DrawLine(DrawingInput& in, DrawingVertex& l, DrawingVertex& r, int32_t y)
		{
			int32_t xmin = std::max((int32_t)l.screen.vertex.x, 0);
			int32_t xmax = std::min((int32_t)r.screen.vertex.x + 1, in.fb.width);

			float invDX = 1.0f / (r.screen.vertex.x - l.screen.vertex.x);
			DrawingVertex current(l.world, l.screen, r.world, r.screen, invDX);
			ScreenVertex& s = current.screen;

			int32_t index = y * in.fb.width + xmin;
			for (int32_t x = l.screen.vertex.x; x < xmax; x++, current.MakeStep())
			{
				if (x < 0) 
					continue;
				// Check if sth closer was already drawn
				if (s.vertex.z < 0.0f || in.fb.zBuffer[y * in.fb.width + x] + FLT_EPSILON < s.vertex.z) continue;

				in.fb.zBuffer[y * in.fb.width + x] = s.vertex.z;
				auto dupa = in.model.GetColor(
					in.wholeWorld, 
					current.world, 
					in.lights, 
					in.texture,
					in.CameraPos
				);
				in.fb.data[y * in.fb.width + x] = Texture::ToRGBA(dupa);
				index++;
			}
		}

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
	};
}
