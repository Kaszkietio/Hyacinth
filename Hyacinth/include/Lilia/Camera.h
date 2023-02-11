#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Lilia
{
	class Camera
	{
	public:
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 Up{ 0.0f, 1.0f, 0.0f };
		static constexpr float Fov = 90.0f;
		static constexpr float FovRad = glm::radians(Fov);
		static constexpr float NearZ = 0.1f;
		static constexpr float FarZ = 100.0f;
		float AspectRatio = 16.0f / 9.0f;

		glm::mat4 ProjMatrix{};
		glm::mat4 ViewMatrix{};

		void OnUpdate(float forward, float up, float right, float yaw)
		{
			constexpr float Speed = 10.0f;
			constexpr float RotataionSpeed = 100.101f;
			glm::vec3 Right = glm::normalize(glm::cross(Front, Up));

			Position += Front * forward * Speed;
			Position += Up * up * Speed;
			Position += Right * right * Speed;

			if (yaw != 0.0f)
			{
				float angle = glm::radians(yaw * RotataionSpeed);

				Front = glm::normalize(glm::vec3(
					glm::cos(angle) * Front.x + glm::sin(angle) * Front.z,
					Front.y,
					-glm::sin(angle) * Front.x + glm::cos(angle) * Front.z
				));
			}

			ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		}

		inline void OnProjectionUpdate(float aspectRatio)
		{
			if (AspectRatio == aspectRatio) return;
			AspectRatio = aspectRatio; ProjMatrix = glm::perspective(FovRad, AspectRatio, NearZ, FarZ);
		}

		inline constexpr glm::mat4 GetProjectionMatrix()
		{
			return ProjMatrix;
		}

		inline constexpr glm::mat4 GetViewMatrix()
		{
			return ViewMatrix;
		}
	};
}
