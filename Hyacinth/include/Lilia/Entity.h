#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Framebuffer.h>

namespace Lilia
{
	class Entity
	{
	public:
		enum class State : int8_t
		{
			Invalid, Valid
		};

		State State = State::Invalid;

		std::string Name{};

		std::vector<Triangle> Triangles{};

		glm::vec3 Position{};
		glm::vec3 Scale{};
		glm::vec3 Rotation{};

		uint32_t TextureId{};

		const glm::mat4 GetModelMatrix() const noexcept
		{ 
			glm::mat4 M = glm::mat4(1.0f);
			M = glm::scale(M, Scale);
			M = glm::translate(M, Position);
			M = glm::rotate(M, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			M = glm::rotate(M, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			M = glm::rotate(M, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			return M;
		}

		void Draw(Framebuffer& fb, const glm::mat4& Proj, const glm::mat4& View);
	};
}
