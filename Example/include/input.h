#pragma once

#include "GLFW/glfw3.h"


namespace Input
{
	constexpr int32_t KEYS_COUNT = 8;

	enum class Keys : int32_t
	{
		W,
		A,
		S,
		D,
		E,
		Q,
		Z,
		C,
	};

	enum class KeyState : int32_t
	{
		Released = 0,
		Pressed = 1
	};
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
