#pragma once
#include "glExtension/glInclude.h"

#include <cstdint>

inline void GLUpdateTexture(int32_t width, int32_t height, uint8_t data[])
{
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0,
		0,
		width,
		height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);
}

inline void GLUpdateFramebuffer(uint32_t readFboId,
	int32_t texWidth,
	int32_t texHeight,
	int32_t winWidth,
	int32_t winHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
	glBlitFramebuffer(0, 0, texWidth, texHeight,
					  0, 0, winWidth, winHeight,
					  GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

inline bool GLTextureResize(GLFWwindow* window,
	int32_t& width,
	int32_t& height)
{
	int32_t curWidth, curHeight;
	glfwGetWindowSize(window, &curWidth, &curHeight);

	// Skip if no resize
	if (width == curWidth && height == curHeight) return false;

	width = curWidth;
	height = curHeight;

	return true;
}
