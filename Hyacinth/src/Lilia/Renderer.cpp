#include <Lilia/Renderer.h>

#include <stbi_include.h>

namespace Lilia
{
	void Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
	{
		UpdateFramebuffer(data, width, height);
		Clear(fb, Scene.BackgroundColor);

		glm::mat4 Proj = Camera.GetProjectionMatrix(float(width) / height);
		glm::mat4 View = Camera.GetViewMatrix();

		for (auto& entity : Scene.Entities)
			DrawEntity(entity, Proj, View);

	}

	void Renderer::DrawEntity(Entity& entity, const glm::mat4& Proj, const glm::mat4& View)
	{
		glm::mat4 Model = entity.GetModelMatrix();
		for (auto& t : entity.Triangles)
		{
			Triangle world = t.ApplyModelMatrix(Model);

			Triangle clip = world;
			clip.ApplyProjViewMatrix(Proj, View);

			std::vector<Triangle> clippedArray = clipper.Clip(clip);

			for (auto& clipped : clippedArray)
			{
				DrawTriangle(entity, world, clipped);
			}

			if (entity.Name == "cube") entity.Rotation.y += 0.1f;
			else
				entity.Rotation.y += 1.0f / glm::length(entity.Scale);
			if (entity.Rotation.y >= 360.0f) entity.Rotation.y -= 360.0f;
		}
	}

	void Renderer::DrawTriangle(Entity& e, Triangle& world, Triangle& clip)
	{
		// to ndc
		clip.ToNDC();
		// to screen
		ScreenTriangle screen = clip.ToScreen(
			fb.width, fb.height,
			Scene.Textures[e.TextureId]
			);

		// sort by y axis
		screen.SortByY();

		// Its simply Flat bottom triangle
		if (screen[0].vertex.y == screen[1].vertex.y)
			screen.DrawFlatBottom(fb);
		// its simply flat top triangle
		else if (screen[1].vertex.y == screen[2].vertex.y)
			screen.DrawFlatTop(fb);
		// more complex case => divide into flat top + flat bottom triangles
		else
		{
			float q = (screen[1].vertex.y - screen[0].vertex.y) / (screen[2].vertex.y - screen[0].vertex.y);
			ScreenVertex m = ScreenVertex::lerp(screen[0], screen[2], q);
			m.vertex.y = screen[1].vertex.y;

			ScreenTriangle flatTop = screen, flatBot = screen;
			flatTop[2] = m;

			flatBot[0] = m;

			flatTop.SortByY();
			flatBot.SortByY();

			flatTop.DrawFlatTop(fb);
			flatBot.DrawFlatBottom(fb);
		}
	}


	Renderer::Renderer()
	{
		// Add textures
		Texture defaultTexture{ .Name = "RedTexture" };
		defaultTexture.data = stbi_load("..\\Resources\\RedTexture.jpg", &defaultTexture.width, &defaultTexture.height, &defaultTexture.channelCount, 0);
		Scene.Textures.push_back(defaultTexture);

		Texture defaultTexture2{ .Name = "GreenTexture" };
		defaultTexture2.data = stbi_load("..\\Resources\\GreenTexture.jpg", &defaultTexture2.width, &defaultTexture2.height, &defaultTexture2.channelCount, 0);
		Scene.Textures.push_back(defaultTexture2);

		Texture smile{ .Name = "Smile" };
		smile.data = stbi_load("..\\Resources\\Smile.jpg",
			&smile.width,
			&smile.height,
			&smile.channelCount,
			0);
		Scene.Textures.push_back(smile);


		// Add Entities
		static const std::string path = "..\\Resources\\";
		Entity triangle = Parser::Parse(path + "triangle.obj", "triangle1");
		auto& t = Scene.Entities.emplace_back(triangle);
		t.TextureId = 0;
		t.Position = glm::vec3(0.0f, 0.0f, -2.0f);
		t.Scale = glm::vec3(2.0f);
		t.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		Entity triangle2 = Parser::Parse(path + "triangle.obj", "triangle2");
		auto& t2 = Scene.Entities.emplace_back(triangle2);
		t2.TextureId = 1;
		t2.Position = glm::vec3(-2.0f, 0.0f, -2.0f);
		t2.Scale = glm::vec3(2.0f);
		t2.Rotation = glm::vec3(90.0f, 90.0f, 0.0f);

		Entity cube = Parser::Parse(path + "cube.obj", "cube");
		auto& t3 = Scene.Entities.emplace_back(cube);
		t3.TextureId = 2;
		t3.Position = glm::vec3(5.0f, 0.0f, -2.0f);
		t3.Scale = glm::vec3(1.0f);
		t3.Rotation = glm::vec3(45.0f, 45.0f, 0.0f);
	}

}