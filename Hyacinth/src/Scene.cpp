#include <Hyacinth/Scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Hyacinth
{
	Scene::Scene()
	{
		Texture defaultTexture{ .Name = "RedTexture" };
		defaultTexture.data = stbi_load("..\\Resources\\RedTexture.png", &defaultTexture.width, &defaultTexture.height, &defaultTexture.channelCount, 0);
		m_textures.push_back(defaultTexture);
	}
}