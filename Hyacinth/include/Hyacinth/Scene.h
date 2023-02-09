#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Hyacinth/Texture.h>
#include <Hyacinth/Entity.h>
#include <Hyacinth/Light.h>


namespace Hyacinth
{
	class Scene
	{
	public:
		glm::vec4 BackgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		std::vector<Light> Lights{};
	private:
		std::vector<Entity> m_entities{};
		std::vector<Texture> m_textures{};

		std::string m_name{};

	public:
		Scene();
		~Scene();

		constexpr const std::string& GetName() const noexcept { return m_name; }
		constexpr std::string& GetName() noexcept { return m_name; }
		void SetName(const std::string& newName) { m_name = newName; }

		constexpr std::vector<Entity>& GetEntities() noexcept { return m_entities; }
		constexpr uint64_t GetEntitiesCount() const noexcept { return m_entities.size(); }
		void AddEntity(
			const Entity& newEntity, 
			uint64_t textureId,
			glm::vec3&& position,
			glm::vec3&& scale,
			glm::vec3&& rotate
		) 
		{ 
			auto& e = m_entities.emplace_back(newEntity);
			if (textureId >= GetTexturesCount()) throw;
			e.SetTextureId(textureId);

			e.SetPosition(std::move(position));
			e.SetScale(std::move(scale));
			e.SetRotation(std::move(rotate));
		}

		constexpr const std::vector<Texture> GetTextures() const noexcept { return m_textures; };
		constexpr uint64_t GetTexturesCount() const noexcept { return m_textures.size(); }
		void AddTexture(Texture&& newTexture) { m_textures.push_back(std::move(newTexture)); }
	};
}
