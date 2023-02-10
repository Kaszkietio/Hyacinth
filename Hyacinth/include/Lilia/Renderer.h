#pragma once
#include <cstdint>
#include <vector>
#include <Lilia/Camera.h>
#include <Lilia/Scene.h>
#include <Lilia/Parser.h>
#include <Lilia/Framebuffer.h>
#include <Lilia/Clipper.h>
#include <Lilia/Shaders/VertexShader.h>
#include <Lilia/Shaders/GeometryShader.h>
#include <Lilia/Shaders/FragmentShader.h>

namespace Lilia
{

	class Renderer
	{
	public:
		
		// Common data
		std::vector<float> zBuffer{};
		Camera Camera{};
		Scene Scene{};
		Framebuffer fb{ .zBuffer = zBuffer };
		std::vector<Light> Lights{};

		// Clippers
		SutherlandHodgmanClipper SHClipper{};
		Clipper& clipper = SHClipper;

		// Shading models
		ConstantShadingModel constModel{};
		PhongShadingModel phongModel{};
		ShadingModel& shadingModel = phongModel;

	public:
		Renderer();

		void OnUpdate(uint32_t data[], int32_t width, int32_t height);

		void DrawEntity(Entity& entity, const glm::mat4& ProjView);

		void DrawTriangle(Entity& e, Triangle& world, Triangle& clip);

		inline void UpdateFramebuffer(uint32_t data[], int32_t width, int32_t height)
		{
			fb.data = data; fb.width = width; fb.height = height;
			int32_t size = fb.width * fb.height;
			for (int i = 0; i < size; i++)
			{
				fb.data[i] = Scene.BackgroundColor;
				fb.zBuffer[i] = FLT_MAX;
			}
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
			int32_t size = fb.width * fb.height;
			for (int i = 0; i < size; i++)
				fb.data[i] = c;
		}
	};
}