#include <Lilia/Renderer.h>

namespace Lilia
{
	void Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
	{
		if ((width & height) == 0) return;
		UpdateFramebuffer(data, width, height);
		Camera& activeCamera = Scene.GetCamera();
		activeCamera.OnProjectionUpdate(float(width) / height);

		glm::mat4 Proj = activeCamera.GetProjectionMatrix();
		glm::mat4 View = activeCamera.GetViewMatrix();
		glm::mat4 ProjView = Proj * View;

		for (auto& entity : Scene.Entities)
			DrawEntity(entity, ProjView);

	}

	void Renderer::DrawEntity(Entity& entity, const glm::mat4& ProjView)
	{
		//VertexData::Payload vertexPayload{};
		//GeometryData::Payload geoPayload{};
		Triangle tmp;
		const Camera& activeCamera = Scene.GetCamera();
		Texture& texture = Scene.Textures[entity.TextureId];
		const glm::mat4& Model = entity.GetModelMatrix();

		// Vertex shader
		VertexShader::Input vertexInput(
			entity.Triangles,
			Model, activeCamera.GetViewMatrix(), activeCamera.GetProjectionMatrix(),
			ProjView
		);
		vertexShader.Process(vertexInput);
		VertexShader::Payload vertexPayload = vertexShader.GetPayload();

		// GeometryShader
		GeometryShader::Input geoInput(
			vertexPayload.world, vertexPayload.clipping,
			clipper
		);
		geometryShader.Process(geoInput);
		GeometryShader::Payload geoPayload = geometryShader.GetPayload();

		// Convert to NDC and screen space
		std::vector<std::vector<ScreenTriangle>> screenTriangles(geoPayload.clipping.size());
		for (int i = 0; i < geoPayload.clipping.size(); ++i)
		{
			screenTriangles[i].resize(geoPayload.clipping[i].size());
			for (int j = 0; j < geoPayload.clipping[i].size(); j++)
			{
				tmp = geoPayload.clipping[i][j].ToNDC();
				screenTriangles[i][j] = tmp.ToScreen(fb.width, fb.height);
			}
		}

		// Fragment shader
		FragmentShader::Input fragInput{
			.fb = fb,
			.wholeWorld = vertexPayload.world,
			.world = geoPayload.world,
			.screenTriangles = screenTriangles,
			.texture = texture,
			.shadingModel = *shadingModel,
			.lights = Scene.Lights,
			.camera = activeCamera,
			.cameraPosition = activeCamera.GetPosition()
		};
		fragmentShader.Process(fragInput);


//		//for (auto& t : entity.Triangles)
//		//{
//		for(int i = 0; i < geoPayload.clipping.size(); i++)
//		{
//			for (int k = 0; k < geoPayload.clipping[i].size(); k++)
//			{
//#if 1
//				// Vertex shader
//				//VertexData::Input vertexInput(t, Model, ProjView);
//				//vertexPayload = VertexShader::Process(vertexInput);
//
//				// Geometry shader
//				//GeometryData::Input geoInput(vertexPayload.World[i], vertexPayload.Clipping[i], clipper);
//				//geoPayload = GeometryShader::Process(geoInput);
//
//#if 0
//				for (auto& clipped : geoPayload.ClippedTriangles)
//				{
//					DrawTriangle(entity, vertexPayload.World, clipped);
//				}
//#else
//
//			// Convert to NDC and to Screen Space
//				std::vector<ScreenTriangle> screenVertice(geoPayload.clipping[i].size());
//				int j = 0;
//				Triangle tmp;
//				for (auto& clipped : geoPayload.clipping[i])
//				{
//					tmp = clipped.ToNDC();
//					screenVertice[j++] = tmp.ToScreen(fb.width, fb.height);
//				}
//				tmp = vertexPayload.clipping[i].ToNDC();
//				ScreenTriangle wholeScreenTriangle = tmp.ToScreen(fb.width, fb.height);
//
//				// Fragment shader
//				FragmentData::Input fragInput(
//					fb,
//					vertexPayload.world[i],
//					wholeScreenTriangle,
//					screenVertice,
//					texture,
//					*shadingModel,
//					Lights,
//					activeCamera.GetPosition());
//				fragmentShader.Process(fragInput);
//#endif
//#else
//				Triangle world = t.ApplyModelMatrix(Model);
//
//				Triangle clip = world.ApplyProjViewMatrix(ProjView);
//				//clip.ApplyProjViewMatrix(ProjView);
//
//				std::vector<Triangle> clippedArray = clipper.Clip(clip);
//
//				for (auto& clipped : clippedArray)
//				{
//					DrawTriangle(entity, world, clipped);
//				}
//#endif
//			}
//
//			// Currently updating here
//			// but gonna change
//			glm::vec3 Rotation = entity.GetRotation();
//			float angle = entity.GetRotationAngle();
//			if (entity.Name == "cube") angle += 0.1f;
//			//else
//			//	Rotation.y += 1.0f;
//			if (angle >= 360.0f) angle -= 360.0f;
//
//			entity.UpdateRotation(Rotation, angle);
//		}
	}

	//void Renderer::DrawTriangle(Entity& e, Triangle& world, Triangle& clip)
	//{
	//	// to ndc
	//	Triangle NDC = clip.ToNDC();
	//	// to screen
	//	ScreenTriangle screen = NDC.ToScreen(
	//		fb.width, fb.height
	//		//,Scene.Textures[e.TextureId]
	//		);

	//	// sort by y axis
	//	screen.SortByY();

	//	// Its simply Flat bottom triangle
	//	if (screen[0].vertex.y == screen[1].vertex.y)
	//		screen.DrawFlatBottom(fb);
	//	// its simply flat top triangle
	//	else if (screen[1].vertex.y == screen[2].vertex.y)
	//		screen.DrawFlatTop(fb);
	//	// more complex case => divide into flat top + flat bottom triangles
	//	else
	//	{
	//		float q = (screen[1].vertex.y - screen[0].vertex.y) / (screen[2].vertex.y - screen[0].vertex.y);
	//		ScreenVertex m = ScreenVertex::lerp(screen[0], screen[2], q);
	//		m.vertex.y = screen[1].vertex.y;

	//		ScreenTriangle flatTop = screen, flatBot = screen;
	//		flatTop[2] = m;
	//		flatBot[0] = m;

	//		flatTop.SortByY();
	//		flatBot.SortByY();

	//		flatTop.DrawFlatTop(fb);
	//		flatBot.DrawFlatBottom(fb);
	//	}
	//}


	Renderer::Renderer()
	{
		MoveCamera(0.0f, 0.0f, 0.0f, 0.001f);
	}

}