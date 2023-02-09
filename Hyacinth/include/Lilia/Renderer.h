#pragma once
#include <cstdint>
#include <vector>
#include <Lilia/Camera.h>
#include <Lilia/Scene.h>
#include <Lilia/Parser.h>
#include <Lilia/Framebuffer.h>
#include <Lilia/Clipper.h>


namespace Lilia
{

	class Renderer
	{
	public:
		
		std::vector<float> zBuffer{};
		Camera Camera{};
		Scene Scene{};
		Framebuffer fb{ .zBuffer = zBuffer };
		SutherlandHodgmanClipper clipper{};

	public:
		Renderer();

		void OnUpdate(uint32_t data[], int32_t width, int32_t height);

		void DrawEntity(Entity& entity, const glm::mat4& Proj, const glm::mat4& View);

		void DrawTriangle(Entity& e, Triangle& world, Triangle& clip);

		inline void UpdateFramebuffer(uint32_t data[], int32_t width, int32_t height)
		{
			fb.data = data; fb.width = width; fb.height = height;
			for (auto& z : zBuffer) z = INFINITY;
		}

		void OnResize(int32_t width, int32_t height)
		{
			zBuffer.resize(static_cast<std::vector<float, std::allocator<float>>::size_type>(width) * height);
		}

		inline void MoveCamera(float forward, float up, float right, float yaw)
		{
			Camera.OnUpdate(forward, up, right, yaw);
		}

		static inline void Clear(const Framebuffer& fb, const glm::vec3& color)
		{
			uint32_t c = Texture::ToRGBA(color);
			for (int i = 0; i < fb.width * fb.height; i++)
				fb.data[i] = c;
		}
	};
}
