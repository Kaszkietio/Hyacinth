#include <Lilia/Shaders/FragmentShader.h>
#include <WindowsNumerics.h>

namespace Lilia
{
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
	inline void FragmentShader::DrawLine(const FragmentData::Input& in)
	{
		int index;
		for (; x < xmax; x++, current.MakeStep(), v.MakeStep())
		{
			if (x < 0) continue;

			index = y * in.Fb.width + x;
			if (current.z < 0.0f || in.Fb.zBuffer[index] + FLT_EPSILON < current.z) continue;

			in.Fb.zBuffer[index] = current.z;
			auto col = in.Model.GetColor(in.World, v.v, in.Lights, in.Tex, in.CameraPos);
			in.Fb.data[index] = Texture::ToRGBA(col);
		}
	}

	void FragmentShader::Process(const FragmentData::Input& input)
	{
		Triangle wholeWorld = input.World;
		ScreenTriangle wholeScreen = input.WholeScreenTriangle;
		Triangle world;
		glm::vec3 bar;

		input.Model.Update(wholeWorld, input.Lights, input.Tex, input.CameraPos);
		for (auto& screen : input.ScreenTriangles)
		{
			screen.SortByY();

			for (int i = 0; i < screen.v.size(); i++)
			{
				bar = Barycentric(
					screen[i].vertex,
					wholeScreen.v[0].vertex,
					wholeScreen.v[1].vertex,
					wholeScreen.v[2].vertex
				);

				world[i].vertex = ApplyBaricentric(
					wholeWorld[0].vertex,
					wholeWorld[1].vertex,
					wholeWorld[2].vertex,
					bar
				);

				world[i].normal = ApplyBaricentric(
					wholeWorld[0].normal,
					wholeWorld[1].normal,
					wholeWorld[2].normal,
					bar
				);

				world[i].texCoord = ApplyBaricentric(
					wholeWorld[0].texCoord,
					wholeWorld[1].texCoord,
					wholeWorld[2].texCoord,
					bar
				);
			}

			//// sort by y axis
			//SortByY(screen, world);

			// Its simply Flat bottom triangle
			if (screen[0].vertex.y == screen[1].vertex.y)
				DrawFlatBottom(input, screen, world);
			// its simply flat top triangle
			else if (screen[1].vertex.y == screen[2].vertex.y)
				DrawFlatTop(input, screen, world);
			// more complex case => divide into flat top + flat bottom triangles
			else
			{
				float q = (screen[1].vertex.y - screen[0].vertex.y) / (screen[2].vertex.y - screen[0].vertex.y);
				ScreenVertex m = ScreenVertex::fastLerpNoY(screen[0], screen[2], q);
				m.vertex.y = screen[1].vertex.y;
				Vertex mv = Vertex::lerp(world[0], world[2], q);
				float q1 = (1 - q) / screen[0].w;
				float q2 = q / screen[2].w;
				mv.normal = glm::normalize((world[0].normal * q1 + world[2].normal * q2) / (q1 + q2));

				ScreenTriangle flatTop = screen, flatBot = screen;
				flatTop[2] = m;
				flatBot[0] = m;
				Triangle flatTopT = world, flatBotT = world;
				flatTopT[2] = mv;
				flatBotT[0] = mv;

				SortByY(flatTop, flatTopT);
				SortByY(flatBot, flatBotT);
				DrawFlatTop(input, flatTop, flatTopT);
				DrawFlatBottom(input, flatBot, flatBotT);
			}
		}
	}
}