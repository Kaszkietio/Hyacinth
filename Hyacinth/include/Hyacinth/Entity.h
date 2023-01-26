#pragma once

#include <vector>

#include <Hyacinth/Face.h>
#include <Hyacinth/Texture.h>
#include <string>

namespace Hyacinth
{
	class Entity
	{
	private:
		std::string Name{};

		std::vector<Face> m_faces{};
		std::vector<glm::vec3> m_vertice{};
		std::vector<glm::vec3> m_normals{};
		std::vector<glm::vec3> m_textureCoords{};

		Texture texture{};
	};
}
