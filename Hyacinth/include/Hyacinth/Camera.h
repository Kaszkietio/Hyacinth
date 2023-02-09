#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hyacinth
{
	class Camera
	{
	public:
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Front{ 0.0f, 0.0f, -1.0f };
		glm::vec3 Up{ 0.0f, 1.0f, 0.0f };

		void OnUpdate(float forward, float up, float right, float yaw)
		{
			constexpr float Speed = 10.0f;
			constexpr float RotataionSpeed = 100.101f;
			glm::vec3 Right = glm::normalize(glm::cross(Front, Up));

			Position += Front * forward * Speed;
			Position += Up * up * Speed;
			Position += Right * right * Speed;

			if (yaw == 0.0f) return;
			float angle = glm::radians(yaw * RotataionSpeed);

			Front = glm::vec3(
				glm::cos(angle) * Front.x + glm::sin(angle) * Front.z,
				Front.y,
				-glm::sin(angle) * Front.x + glm::cos(angle) * Front.z
				);
		}
	};
}
