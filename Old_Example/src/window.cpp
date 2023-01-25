#include <hyacinth/window.h>
#include <iostream>

namespace Hyacinth
{
	Window* Window::m_instance = nullptr;
	GLFWwindow* Window::m_window{ nullptr };
	int32_t Window::m_width = Window::INIT_WIDTH;
	int32_t Window::m_height = Window::INIT_HEIGHT;

	Window::Window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(Window::INIT_WIDTH, Window::INIT_HEIGHT, Window::TITLE, nullptr, nullptr);
		if (m_window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		
		glViewport(0, 0, Window::INIT_WIDTH, Window::INIT_HEIGHT);

		glfwSetFramebufferSizeCallback(m_window, OnResize);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::OnResize(GLFWwindow* window, int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
		Window::m_width = width;
		Window::m_height = height;
	}

	void Window::Run()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			// input
			//processInput();

			// rendering commands here
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			
			// check and call events and swap buffers
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}
	}

}