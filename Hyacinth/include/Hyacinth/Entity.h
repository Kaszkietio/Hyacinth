#pragma once

#include <vector>

#include <Hyacinth/Triangle.h>
#include <Hyacinth/Texture.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hyacinth
{
	class Entity
	{
	public:
		enum class State : int8_t
		{
			Invalid, Valid
		};

	private:
		State m_state = State::Invalid;

		std::string m_name{};

		std::vector<Triangle> m_triangles{};

#ifndef FUN
		std::vector<glm::vec4> m_vertice{};
		std::vector<glm::vec4> m_normals{};
		std::vector<glm::vec2> m_textureCoords{};
#endif

		glm::vec3 m_position{};
		glm::vec3 m_scale{};
		glm::vec3 m_rotation{};

		uint32_t m_textureId{};
	public:
		constexpr const std::string& GetName() const noexcept { return m_name; }
		constexpr std::string& GetName() noexcept { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		constexpr const std::vector<Triangle>& GetMesh() const noexcept { return m_triangles; }
		constexpr std::vector<Triangle>& GetMesh() noexcept { return m_triangles; }
		void SetMesh(std::vector<Triangle>&& newFaces) { m_triangles = std::move(newFaces); }

		constexpr uint32_t GetTextureId() const noexcept{ return m_textureId; }
		void SetTextureId(uint32_t newTextureId) { m_textureId = newTextureId; }

		constexpr const bool IsValidState() const noexcept { return m_state == State::Valid; }
		constexpr const State GetState() const noexcept { return m_state; }
		void SetState(State newState) noexcept { m_state = newState; }

		const glm::mat4 GetModelMatrix() const noexcept
		{ 
			glm::mat4 M = glm::mat4(1.0f);
			M = glm::translate(M, m_position);
			M = glm::rotate(M, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			M = glm::rotate(M, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			M = glm::rotate(M, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			M = glm::scale(M, m_scale);

			return M;
		}

		constexpr const glm::vec3 GetPosition() const noexcept { return m_position; }
		void SetPosition(glm::vec3&& newPos) { m_position = std::move(newPos); }

		constexpr const glm::vec3 GetScale() const noexcept { return m_scale; }
		void SetScale(glm::vec3&& newScale) { m_scale = std::move(newScale); }

		constexpr const glm::vec3 GetRotation() const noexcept { return m_rotation; }
		void SetRotation(glm::vec3&& newRot) { m_rotation = std::move(newRot); }
	};
}
