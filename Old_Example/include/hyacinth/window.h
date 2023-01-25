#pragma once

#include "hyacinth/include.h"

#include <memory>

namespace Hyacinth
{
	class Window
	{
	public:
		static constexpr int32_t INIT_WIDTH = 800;
		static constexpr int32_t INIT_HEIGHT = 600;

		static constexpr const char* TITLE = "Hyacinth";

	private:
		static Window* m_instance;
		static GLFWwindow* m_window;
		static int32_t m_width;
		static int32_t m_height;

	private:
		Window();
	public:
		static Window* GetInstance() { if (m_window == nullptr) { m_instance = new Window(); } return m_instance; }
		static void OnResize(GLFWwindow* window, int32_t width, int32_t height);
		constexpr int32_t GetWidth() const noexcept { return m_width; }
		constexpr int32_t GetHeight() const noexcept { return m_height; }

		void Run();;



		~Window();
	};
}
