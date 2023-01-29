#include <Hyacinth/Renderer.h>
#include <Hyacinth/utils.h>
#include <Hyacinth/rendering.h>

#include <glm/vec3.hpp>
#include <Hyacinth/ObjParser.h>

#include <algorithm>

Hyacinth::Renderer::Renderer()
{
	static const std::string path = "..\\Resources\\";
	Entity triangle = ObjParser::Parse(path + "triangle.obj", "triangle1");
	m_scene.AddEntity(
		triangle,
		0,
		glm::vec3(2.0f, 2.0f, -2.0f),
		glm::vec3(2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
}

Hyacinth::Renderer::~Renderer()
{
}

void Hyacinth::Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
{
	auto backColor = to_rgba(m_scene.BackgroundColor);

	// Clear background
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			data[y * width + x] = backColor;
		}
	}

	// tmp array
	std::array<glm::vec4, 3> vertice{};
	std::array<glm::vec4, 3> normals{};

	// camera
	static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// View
	static glm::mat4 View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// Projection
	constexpr float fov = 90.0f;
	const float aspectRatio = (float)width / height;
	constexpr float nearZ = 0.1f;
	constexpr float farZ = 100.0f;
	glm::mat4 Proj = glm::perspective(glm::radians(90.0f), aspectRatio, nearZ, farZ);
	glm::mat4 Res = Proj * View;

	// Textures
	auto& Textures = m_scene.GetTextures();

#ifdef FUN
	// Draw entities
	for (auto& entity : m_scene.GetEntities())
	{
		if (!entity.IsValidState()) continue;

		glm::mat4 Model = entity.GetModelMatrix();
		glm::mat4 Matrix = Res * Model;
		glm::mat4 NormalMatrix = glm::inverse(glm::transpose(Model));

		// Draw triangle
		for (auto triangle : entity.GetMesh())
		{
			std::for_each(triangle.vertice.begin(), triangle.vertice.end(),
			[width, height, &Matrix](glm::vec4& a)
			{
				a = Matrix * a;
			});

			//auto dupa = ClipTriangle(triangle);

			for (auto& t : ClipTriangle(triangle))
			{
				std::for_each(t.vertice.begin(), t.vertice.end(),
				[width, height, &Matrix](glm::vec4& a)
				{
					a.x = (a.x / a.w + 1.0f) * 0.5f * width;
					a.y = (a.y / a.w + 1.0f) * 0.5f * height;
					a.z = (a.z / a.w + 1.0f) * 0.5f;
					a.w = 1.0f;
				});

				DrawTriangle(data, width, height, t, Textures[entity.GetTextureId()]);
			}
		}
		glm::vec3 newRotation = entity.GetRotation();
		newRotation.y += 1.0f;
		if (newRotation.y >= 360.0f) newRotation.y -= 360.0f;
		entity.SetRotation(std::move(newRotation));
	}
#else
	// Draw entities
	for (auto& entity : m_scene.GetEntities())
	{
		if (!entity.IsValidState()) continue;

		auto& v = entity.GetVertice();
		auto& n = entity.GetNormals();
		glm::mat4 Model = entity.GetModelMatrix();
		glm::mat4 Matrix = Res * Model;
		glm::mat4 NormalMatrix = glm::inverse(glm::transpose(Model));

		// Draw triangle
		for (auto& triangle : entity.GetMesh())
		{
			for (int i = 0; i < 3; i++)
			{
				vertice[i] = Matrix * v[triangle.vertexId[i]];
				normals[i] = NormalMatrix * n[triangle.normalId[i]];
			}

			std::for_each(vertice.begin(), vertice.end(), 
				[width, height](glm::vec4& a) 
				{
					a.x = (a.x / a.w + 1.0f) * 0.5f * width; 
					a.y = (a.y / a.w + 1.0f) * 0.5f * height; 
				});

			DrawTriangle(data, width, height, vertice);
		}
		glm::vec3 newRotation = entity.GetRotation();
		newRotation.y += 1.0f;
		if (newRotation.y >= 360.0f) newRotation.y -= 360.0f;
		entity.SetRotation(std::move(newRotation));
	}
#endif
}
