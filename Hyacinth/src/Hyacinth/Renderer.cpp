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
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	//Entity triangle2 = ObjParser::Parse(path + "triangle.obj", "triangle2");
	//m_scene.AddEntity(
	//	triangle2,
	//	1,
	//	glm::vec3(0.0f, 0.0f, -10.0f),
	//	glm::vec3(2.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f)
	//);
	/*Entity cube = ObjParser::Parse(path + "cube.obj", "cube");
	m_scene.AddEntity(
		cube,
		0,
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);*/

	m_scene.Lights.push_back(
		{
			//.Position = glm::vec4(0.0f, 4.0f, 0.0f, 1.0f),
			//.ForwardDirection = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)
			.Position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			.ForwardDirection = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)
		}
	);
}

Hyacinth::Renderer::~Renderer()
{
}

void Hyacinth::Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
{
	if (width == 0 || height == 0) return;
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
	glm::vec3& cameraPos = m_camera.Position;
	glm::vec3& cameraFront = m_camera.Front;
	glm::vec3& cameraUp = m_camera.Up;


	// View
	glm::mat4 View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// Projection
	constexpr float fov = 90.0f;
	const float aspectRatio = (float)width / height;
	constexpr float nearZ = 0.1f;
	constexpr float farZ = 100.0f;
	glm::mat4 Proj = glm::perspective(glm::radians(90.0f), aspectRatio, nearZ, farZ);
	glm::mat4 Res = Proj * View;

	// Textures
	auto& Textures = m_scene.GetTextures();

	// Z-buffer
	for (auto& f : zBuffer)
		f = INFINITY;

	// lights
	//static std::vector<Light> lights;
	//if (lights.size() != m_scene.Lights.size())
	//	lights.resize(m_scene.Lights.size());

	//for (int i = 0; i < lights.size(); i++)
	//{
	//	lights[i] = m_scene.Lights[i];
	//	lights[i].Position = Res * glm::mat4(1.0f) *  lights[i].Position;
	//	lights[i].ForwardDirection = Res *glm::mat4(1.0f) * lights[i].ForwardDirection;

	//	// To NDC
	//	lights[i].Position.x /= lights[i].Position.w;
	//	lights[i].Position.y /= lights[i].Position.w;
	//	lights[i].Position.z /= lights[i].Position.w;
	//	lights[i].Position.w /= lights[i].Position.w;
	//}

	////////////////////////////////////////////
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
			for(int i = 0; i < triangle.vertice.size(); i++)
			{
				triangle.vertice[i] = Matrix * triangle.vertice[i];
				triangle.normals[i] = NormalMatrix * triangle.normals[i];
			}

			for (auto& t : ClipTriangle(triangle))
			{
				// to NDC
				for (int i = 0; i < t.vertice.size(); i++)
				{
					t.vertice[i] /= t.vertice[i].w;
				}

				DrawTriangle(data, width, height, 
					t, Textures[entity.GetTextureId()],
					zBuffer, backColor);
			}
		}
		glm::vec3 newRotation = entity.GetRotation();
		//if (entity.GetName() == "triangle1")
		//{
		//	newRotation.x += 0.1f;
		//	if (newRotation.x >= 360.0f) newRotation.x -= 360.0f;
		//}
		//else if (entity.GetName() == "triangle2")
		//{
		//	newRotation.z += 0.1f;
		//	if (newRotation.z >= 360.0f) newRotation.z -= 360.0f;
		//}
		entity.SetRotation(std::move(newRotation));
	}
}
