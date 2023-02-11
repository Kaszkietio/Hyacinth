#pragma once
#include <vector>

#include <Lilia/Entity.h>
#include <Lilia/Light.h>
#include <Lilia/Texture.h>

namespace Lilia
{
	class Scene
	{
	protected:
		static const std::string path;
		static const std::string textureSuffix;
		static const std::string objSuffix;
	public:
		uint32_t BackgroundColor = 0xff000000;
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
