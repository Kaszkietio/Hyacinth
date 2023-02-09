#include <Hyacinth/Scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Hyacinth
{
	Scene::Scene()
	{
		Texture defaultTexture{ .Name = "RedTexture" };
		defaultTexture.data = stbi_load("..\\Resources\\RedTexture.jpg", &defaultTexture.width, &defaultTexture.height, &defaultTexture.channelCount, 0);
		m_textures.push_back(defaultTexture);

		Texture defaultTexture2{ .Name = "GreenTexture" };
		defaultTexture2.data = stbi_load("..\\Resources\\GreenTexture.jpg", &defaultTexture2.width, &defaultTexture2.height, &defaultTexture2.channelCount, 0);
		m_textures.push_back(defaultTexture2);

		Texture defaultTexture3{ .Name = "GreenTexture" };
		defaultTexture3.data = stbi_load("..\\Resources\\PurpuleTexture.jpg", &defaultTexture2.width, &defaultTexture2.height, &defaultTexture2.channelCount, 0);
		m_textures.push_back(defaultTexture3);

		Texture smile{ .Name = "Smile" };
		smile.data = stbi_load("..\\Resources\\Smile.jpg",
			&smile.width,
			&smile.height,
			&smile.channelCount,
			0);

		m_textures.push_back(smile);
	}
	Scene::~Scene()
	{
		for (auto& tex : m_textures)
		{
			stbi_image_free(tex.data);
		}
	}
}