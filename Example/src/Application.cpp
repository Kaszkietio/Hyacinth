#include "Application.h"

#include "glExtension/glErrorHandling.h"
#include "Logger.h"
#include "glExtension/glUtils.h"

#include "glm/common.hpp"
#include "glm/gtc/random.hpp"

#include <iostream>
#include <iomanip>
#include <random>

Input::KeyState Application::Keys[Input::KEYS_COUNT]{ (Input::KeyState)0 };

Application::Application(
    int32_t width, 
    int32_t height)
{
	// Check if valid width and height (width and height not 0)
	if (width & height)
	{
		m_window.Width = width;
		m_window.Height = height;
	}

    m_renderer.OnResize(m_window.Width, m_window.Height);

    // Check if setup secceeded
    int res = Setup();
    if (res)
        Error("window setup failure\n");

    // TODO: poprawic kulturalnie
    m_data = std::make_unique<uint8_t[]>(static_cast<size_t>(4) * m_window.Width * m_window.Height);

	CreateTextureAndFramebuffer();

    FillSceneData();
}

Application::~Application()
{
    glfwTerminate();
}

int Application::Run()
{
	double curTime = glfwGetTime();
	double nextTime;

    std::cout << std::fixed << std::setprecision(4) << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_window.ptr))
    {
        // Resize
        if (GLTextureResize(m_window.ptr, m_window.Width, m_window.Height))
            ResizeBuffers();

        nextTime = glfwGetTime();
#if 1
        std::cout << 1 / (nextTime - curTime) << " FPS\n";
#endif
        UpdateCamera(nextTime - curTime);
        UpdateShader();
        curTime = nextTime;
        
        // Update frame
        CalculateFrame();
        GLUpdateTexture(m_window.Width,
            m_window.Height,
            m_data.get()
        );
        GLUpdateFramebuffer(m_readFboId,
            m_window.Width, m_window.Height,
            m_window.Width, m_window.Height);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window.ptr);

        /* Poll for and process events */
        glfwPollEvents();

    }

    return 0;
}

int Application::Setup()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    m_window.ptr = glfwCreateWindow(m_window.Width, 
                                    m_window.Height, 
                                    m_window.title,
                                    NULL, 
                                    NULL);
    if (!m_window.ptr)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window.ptr);

    // Check if initializing window succeeded
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    // Setup message callback function
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MyMessageCallback, 0);

    // Setup key callback function
    glfwSetKeyCallback(m_window.ptr, Input::key_callback);

    return 0;
}


void Application::CreateTextureAndFramebuffer()
{
    // Create texture
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        m_window.Width,
        m_window.Height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
		(const void*)m_data.get()
        );

    // Generate framebuffer
    glGenFramebuffers(1, &m_readFboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_readFboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, 
                           GL_COLOR_ATTACHMENT0, 
                           GL_TEXTURE_2D, 
                           m_textureId, 
                           0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

}

void Application::FillSceneData()
{
}

void Application::CalculateFrame()
{
	uint8_t* imageData = m_data.get();
    int32_t& width = m_window.Width;
    int32_t& height = m_window.Height;
    m_renderer.OnUpdate((uint32_t*)imageData, width, height);
}

void Application::ResizeBuffers()
{
    m_data = std::make_unique<uint8_t[]>(static_cast<size_t>(4) * m_window.Width * m_window.Height);
    m_renderer.OnResize(m_window.Width, m_window.Height);

    glTexImage2D( GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		m_window.Width,
		m_window.Height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(const void*)m_data.get()
    );
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS && action != GLFW_RELEASE)
        return;

	switch (key)
	{
	case GLFW_KEY_W:
		Application::Keys[(int32_t)Keys::W] = (Input::KeyState)action;
		break;
	case GLFW_KEY_A:
		Application::Keys[(int32_t)Keys::A] = (Input::KeyState)action;
		break;
	case GLFW_KEY_S:
		Application::Keys[(int32_t)Keys::S] = (Input::KeyState)action;
		break;
	case GLFW_KEY_D:
		Application::Keys[(int32_t)Keys::D] = (Input::KeyState)action;
		break;
	case GLFW_KEY_E:
		Application::Keys[(int32_t)Keys::E] = (Input::KeyState)action;
		break;
	case GLFW_KEY_Q:
		Application::Keys[(int32_t)Keys::Q] = (Input::KeyState)action;
		break;
    case GLFW_KEY_Z:
		Application::Keys[(int32_t)Keys::Z] = (Input::KeyState)action;
		break;
    case GLFW_KEY_C:
		Application::Keys[(int32_t)Keys::C] = (Input::KeyState)action;
		break;
    case GLFW_KEY_1:
		Application::Keys[(int32_t)Keys::ONE] = (Input::KeyState)action;
		break;
    case GLFW_KEY_2:
		Application::Keys[(int32_t)Keys::TWO] = (Input::KeyState)action;
		break;
    case GLFW_KEY_3:
		Application::Keys[(int32_t)Keys::THREE] = (Input::KeyState)action;
		break;
	default:
		break;
	}
}

void Application::UpdateCamera(float ts)
{
    float dx = 0.0f, dy = 0.0f, dz = 0.0f;
    float rot = 0.0f;

    // W
    Input::KeyState state = Application::Keys[(int32_t)Input::Keys::W];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dz += ts;

    // S
    state = Application::Keys[(int32_t)Input::Keys::S];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dz -= ts;

    // A
    state = Application::Keys[(int32_t)Input::Keys::A];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dx -= ts;

    // D
    state = Application::Keys[(int32_t)Input::Keys::D];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dx += ts;

    // E
    state = Application::Keys[(int32_t)Input::Keys::E];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dy += ts;

    // Q
    state = Application::Keys[(int32_t)Input::Keys::Q];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        dy -= ts;

    // Z
    state = Application::Keys[(int32_t)Input::Keys::Z];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        rot += ts;

    // C
    state = Application::Keys[(int32_t)Input::Keys::C];
    if ((int32_t)state) // state == Input::KeyState::Pressed
        rot -= ts;

    m_renderer.MoveCamera(dz, dy, dx, rot);
}

void Application::UpdateShader()
{
    m_renderer.UpdateShader(
        (int32_t)Keys[(int32_t)Input::Keys::ONE],
        (int32_t)Keys[(int32_t)Input::Keys::TWO],
        (int32_t)Keys[(int32_t)Input::Keys::THREE]
    );
}
