#pragma once
#include "glExtension/glInclude.h"

#include "Hyacinth/Renderer.h"

#include "input.h"

#include <cstdint>
#include <memory>
#include <string>

class Application
{
////////////////////////////////////// FIELDS ////////////////////////////
public:
	struct Window
	{
		GLFWwindow* ptr = nullptr;
		//int32_t Width = 1024;
		//int32_t Height = 768;
		//int32_t Width = 1920;
		//int32_t Height = 1080;
		int32_t Width = 1000;
		int32_t Height = 1000;
		const char* title = "SphereReyCaster";
	};

	static Input::KeyState Keys[Input::KEYS_COUNT];
private:
	//LumenOpus::Renderer m_renderer{};
	Hyacinth::Renderer m_renderer{};

	std::unique_ptr<uint8_t[]> m_data;

	Window m_window;
	uint32_t m_textureId;
	uint32_t m_readFboId;
////////////////////////////////////// MEMBER FUNCTIONS ////////////////////////////
public:
	Application(
		//const Apollo::Settings& rendererSettings, 
		int32_t width = 0,
		int32_t height = 0);
	~Application();

	/// <summary>
	/// Main loop of the application
	/// </summary>
	/// <returns>Returns 0 if completed successfully, error code otherwise.</returns>
	int Run();

private:
	/// <summary>
	/// Member function which sets up window
	/// </summary>
	/// <returns></returns>
	int Setup();

	/// <summary>
	/// Member function creating texture and framebuffer to display calculated image
	/// </summary>
	void CreateTextureAndFramebuffer();

	/// <summary>
	/// Member function filling scene data with data
	/// </summary>
	void FillSceneData();

	/// <summary>
	/// Member function which calculates current frame
	/// </summary>
	void CalculateFrame();

	void ResizeBuffers();

	void UpdateCamera(float ts);
};

