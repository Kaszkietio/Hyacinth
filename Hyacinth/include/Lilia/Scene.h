#pragma once
#include <vector>

#include <Lilia/Entity.h>
#include <Lilia/Light.h>
#include <Lilia/Texture.h>

namespace Lilia
{
	class Scene
	{
	public:
		glm::vec3 BackgroundColor{ 0.0f, 0.0f, 0.0f };
		std::vector<Entity> Entities;
		std::vector<Light> Lights;
		std::vector<Texture> Textures{};

		~Scene()
		{
			for (auto& t : Textures)
				free(t.data);
		}
	};
}
