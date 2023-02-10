#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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
		uint32_t TextureId{};

		std::vector<Triangle> Triangles{};
	private:
		glm::vec3 Position{};
		glm::mat4 TranslationMatrix{};
		glm::vec3 Scale{};
		glm::mat4 ScaleMatrix{};
		glm::vec3 Rotation{};
		glm::mat4 RotationMatrix{};
		float RotationAngle{};

		glm::mat4 ModelMatrix{};
		bool ModelUpdated{ false };

	public:
		Entity()
		{
			UpdateModelMatrix();
		}

		const glm::mat4& GetModelMatrix()  
		{ 
			if(!ModelUpdated) UpdateModelMatrix();
			return ModelMatrix;
		}

		void UpdateModelMatrix()
		{
			//glm::mat4& M = ModelMatrix;
			//M = glm::mat4(1.0f);
			//M = glm::scale(M, Scale);
			//M = glm::rotate(M, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			//M = glm::rotate(M, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			//M = glm::rotate(M, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			//M = glm::translate(M, Position);

			ScaleMatrix = glm::scale(Scale);
			glm::quat MyQuat = glm::angleAxis(RotationAngle, Rotation);
			RotationMatrix = glm::toMat4(MyQuat);
			TranslationMatrix = glm::translate(glm::mat4(1.0f), Position);
			ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

			ModelUpdated = true;
		}

		inline void UpdatePosition(const glm::vec3& position)
		{
			Position = position;
			ModelUpdated = false;
		}

		inline void UpdateScale(const glm::vec3& scale)
		{
			Scale = scale;
			ModelUpdated = false;
		}

		inline void UpdateRotation(const glm::vec3& rotation, float angle)
		{
			Rotation = rotation;
			RotationAngle = glm::radians(angle);
			ModelUpdated = false;
		}

		constexpr glm::vec3 GetPosition()
		{
			return Position;
		}

		constexpr glm::vec3 GetScale()
		{
			return Scale;
		}

		constexpr glm::vec3 GetRotation()
		{
			return Rotation;
		}

		constexpr float GetRotationAngle()
		{
			return glm::degrees(RotationAngle);
		}

		void Draw(Framebuffer& fb, const glm::mat4& Proj, const glm::mat4& View);
	};
}
