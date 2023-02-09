#pragma once
#include <cstdint>
#include <Hyacinth/Scene.h>
#include "Camera.h"

namespace Hyacinth
{
	class Renderer
	{
	private:
		Scene m_scene{};
		Camera m_camera{};
		std::vector<float> zBuffer{};

	public:
		Renderer();
		~Renderer();

		void OnUpdate(uint32_t data[], int32_t width, int32_t height);

		void OnResize(int32_t width, int32_t height)
		{
			zBuffer.resize(static_cast<std::vector<float, std::allocator<float>>::size_type>(width) * height);
		}

		inline void MoveCamera(float forward, float up, float right, float yaw)
		{
			m_camera.OnUpdate(forward, up, right, yaw);
		}
	};
}
