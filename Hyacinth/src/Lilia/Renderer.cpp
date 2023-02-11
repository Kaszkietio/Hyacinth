#include <Lilia/Renderer.h>

#include <stbi_include.h>

namespace Lilia
{
	void Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
	{
		if ((width & height) == 0) return;
		UpdateFramebuffer(data, width, height);
		//Clear(fb, Scene.BackgroundColor);

		glm::mat4 Proj = Camera.GetProjectionMatrix(float(width) / height);
		glm::mat4 View = Camera.GetViewMatrix();
		glm::mat4 ProjView = Proj * View;

		for (auto& entity : Scene.Entities)
			DrawEntity(entity, ProjView);

	}

	void Renderer::DrawEntity(Entity& entity, const glm::mat4& ProjView)
	{
		VertexData::Payload vertexPayload{};
		GeometryData::Payload geoPayload{};
		const glm::mat4& Model = entity.GetModelMatrix();
		Texture& tex = Scene.Textures[entity.TextureId];

		for (auto& t : entity.Triangles)
		{
#if 1
			// Vertex shader
			VertexData::Input vertexInput(t, Model, ProjView);
			vertexPayload = VertexShader::Process(vertexInput);

			// Geometry shader
			GeometryData::Input geoInput(vertexPayload.World, vertexPayload.Clipping, clipper);
			geoPayload = GeometryShader::Process(geoInput);

#if 0
			for (auto& clipped : geoPayload.ClippedTriangles)
			{
				DrawTriangle(entity, vertexPayload.World, clipped);
			}
#else

			// Convert to NDC and to Screen Space
			std::vector<ScreenTriangle> screenVertice(geoPayload.ClippedTriangles.size());
			int i = 0;
			for (auto& clipped : geoPayload.ClippedTriangles)
			{
				screenVertice[i++] = clipped.ToNDC().ToScreen(fb.width, fb.height);
			}
			ScreenTriangle wholeScreenTriangle = vertexPayload.Clipping.ToNDC().ToScreen(fb.width, fb.height);

			// Fragment shader
			FragmentData::Input fragInput(
				fb, 
				geoPayload.World, 
				wholeScreenTriangle, 
				screenVertice, 
				tex, 
				*shadingModel, 
				Lights,
				Camera.Position);
			fragmentShader.Process(fragInput);
#endif
#else
			Triangle world = t.ApplyModelMatrix(Model);

			Triangle clip = world.ApplyProjViewMatrix(ProjView);
			//clip.ApplyProjViewMatrix(ProjView);

			std::vector<Triangle> clippedArray = clipper.Clip(clip);

			for (auto& clipped : clippedArray)
			{
				DrawTriangle(entity, world, clipped);
			}
#endif

			// Currently updating here
			// but gonna change
			glm::vec3 Rotation = entity.GetRotation();
			float angle = entity.GetRotationAngle();
			if (entity.Name == "cube") angle += 0.1f;
			//else
			//	Rotation.y += 1.0f;
			if (angle >= 360.0f) angle -= 360.0f;

			entity.UpdateRotation(Rotation, angle);
		}
	}

	void Renderer::DrawTriangle(Entity& e, Triangle& world, Triangle& clip)
	{
		// to ndc
		Triangle NDC = clip.ToNDC();
		// to screen
		ScreenTriangle screen = NDC.ToScreen(
			fb.width, fb.height
			//,Scene.Textures[e.TextureId]
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
		MoveCamera(0.0f, 0.0f, 0.0f, 0.001f);

		// Add textures
		Texture red{ .Name = "RedTexture" };
		red.data = stbi_load("..\\Resources\\RedTexture.jpg", &red.width, &red.height, &red.channelCount, 4);
		Scene.Textures.push_back(red);

		Texture green{ .Name = "GreenTexture" };
		green.data = stbi_load("..\\Resources\\GreenTexture.jpg", &green.width, &green.height, &green.channelCount, 4);
		Scene.Textures.push_back(green);

		Texture blue{ .Name = "BlueTexture" };
		blue.data = stbi_load("..\\Resources\\BlueTexture.jpg", &blue.width, &blue.height, &blue.channelCount, 4);
		Scene.Textures.push_back(blue);

		Texture purple{ .Name = "PurpleTexture" };
		purple.data = stbi_load("..\\Resources\\PurpleTexture.jpg", &purple.width, &purple.height, &purple.channelCount, 4);
		Scene.Textures.push_back(purple);

		Texture smile{ .Name = "Smile" };
		smile.data = stbi_load("..\\Resources\\Smile.jpg",
			&smile.width,
			&smile.height,
			&smile.channelCount,
			4);
		Scene.Textures.push_back(smile);


		// Add Entities
		static const std::string path = "..\\Resources\\";
		Entity triangle = Parser::Parse(path + "triangle.obj", "triangle1");
		auto& t = Scene.Entities.emplace_back(triangle);
		t.TextureId = 0;
		t.UpdatePosition(glm::vec3(0.0f, 0.0f, -1.0f));
		t.UpdateScale(glm::vec3(2.0f));
		t.UpdateRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		t.UpdateModelMatrix();

		Entity triangle2 = Parser::Parse(path + "triangle.obj", "triangle2");
		auto& t2 = Scene.Entities.emplace_back(triangle2);
		t2.TextureId = 2;
		t2.UpdatePosition(glm::vec3(-2.0f, 0.0f, -2.0f));
		t2.UpdateScale(glm::vec3(2.0f));
		t2.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 315.0f);
		t2.UpdateModelMatrix();

		Entity cube = Parser::Parse(path + "cube.obj", "cube");
		auto& t4 = Scene.Entities.emplace_back(cube);
		t4.TextureId = 3;
		t4.UpdatePosition(glm::vec3(5.0f, 0.0f, -2.0f));
		t4.UpdateScale(glm::vec3(1.0f));
		t4.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t4.UpdateModelMatrix();

		Entity sphere = Parser::Parse(path + "sphere.obj", "sphere");
		auto& t5 = Scene.Entities.emplace_back(sphere);
		t5.TextureId = 3;
		t5.UpdatePosition(glm::vec3(4.0f, 2.0f, 8.0f));
		t5.UpdateScale(glm::vec3(1.0f));
		t5.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t5.UpdateModelMatrix();

		Entity smoothSphere = Parser::Parse(path + "smoothSphere.obj", "smoothSphere");
		auto& t8 = Scene.Entities.emplace_back(smoothSphere);
		t8.TextureId = 3;
		t8.UpdatePosition(glm::vec3(-3.0f, 2.0f, 8.0f));
		t8.UpdateScale(glm::vec3(1.0f));
		t8.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t8.UpdateModelMatrix();

		Entity plane = Parser::Parse(path + "plane.obj", "plane");
		auto& t6 = Scene.Entities.emplace_back(plane);
		t6.TextureId = 1;
		t6.UpdatePosition(glm::vec3(0.0f, -2.0f, 0.0f));
		t6.UpdateScale(glm::vec3(1.0f));
		t6.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t6.UpdateModelMatrix();

		//Entity triangle3 = Parser::Parse(path + "triangle.obj", "triangle3");
		//auto& t3 = Scene.Entities.emplace_back(triangle3);
		//t3.TextureId = 1;
		//t3.UpdatePosition(glm::vec3(0.0f, -3.0f, -50.0f));
		//t3.UpdateScale(glm::vec3(100.0f));
		//t3.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f),  315.0f);
		//t3.UpdateModelMatrix();


		//Entity testplane = Parser::Parse(path + "testplane.obj", "testplane");
		//auto& t7 = Scene.Entities.emplace_back(testplane);
		//t7.TextureId = 3;
		//t7.UpdatePosition(glm::vec3(0.0f, 0.0f, -2.0f));
		//t7.UpdateScale(glm::vec3(1.0f));
		//t7.UpdateRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		//t7.UpdateModelMatrix();
	}

}