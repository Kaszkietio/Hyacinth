#include "Hyacinth/Renderer.h"
#include "Hyacinth/utils.h"
#include "Hyacinth/rendering.h"

#include <glm/vec3.hpp>

Hyacinth::Renderer::Renderer()
{
}

Hyacinth::Renderer::~Renderer()
{
}

void Hyacinth::Renderer::OnUpdate(uint32_t data[], int32_t width, int32_t height)
{
	static float z = -0.1f;
	z -= 0.1f;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			data[y * width + x] = to_rgba(glm::vec3(0.2f, 0.3f, 0.3f));
		}
	}
	//glm::vec3 a(
	//	((0.0f) + 1.0f) * 0.5f * width,
	//	((-0.5f) + 1.0f) * 0.5f * height,
	//	0.0f);
	//glm::vec3 b(
	//	((0.5f) + 1.0f) * 0.5f * width,
	//	((0.2f) + 1.0f) * 0.5f * height,
	//	0.0f);
	//glm::vec3 c(
	//	((-0.1f) + 1.0f) * 0.5f * width,
	//	((0.35f) + 1.0f) * 0.5f * height,
	//	0.0f);
	
	//glm::vec3 a(
	//	((0.0f) + 1.0f) * 0.5f * width,
	//	((-0.5f) + 1.0f) * 0.5f * height,
	//	//((0.0f) + 1.0f) * 0.5f * height,
	//	0.0f);
	//glm::vec3 b(
	//	((-0.5f) + 1.0f) * 0.5f * width,
	//	((0.5f) + 1.0f) * 0.5f * height,
	//	0.0f);
	//glm::vec3 c(
	//	((0.5f) + 1.0f) * 0.5f * width,
	//	((0.5f) + 1.0f) * 0.5f * height,
	//	0.0f);

	glm::vec4 c(
		(0.0f),
		(0.5f),
		0.0f,
		1.0f
	);
	glm::vec4 a(
		(-0.5f),
		(-0.5f),
		0.0f,
		1.0f
	);
	glm::vec4 b(
		(0.5f),
		(-0.5f),
		0.0f,
		1.0f
	);

	//glm::mat4 trans = glm::mat4(1.0f);
	//float rads = glm::radians(rotate);
	//trans = glm::rotate(trans, rads, glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));

	//a = trans * a;
	//b = trans * b;
	//c = trans * c;

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(90.0f), (float)width / height, 0.1f, 100.0f);

	glm::mat4 tmp2 = projection * view;

	glm::mat4 tmp = projection * view * model;

	a = projection * view * model * a;
	b = projection * view * model * b;
	c = projection * view * model * c;

	a.x = (a.x/a.w + 1.0f) * 0.5f * width;
	a.y = (a.y/a.w + 1.0f) * 0.5f * height;
	b.x = (b.x/b.w + 1.0f) * 0.5f * width;
	b.y = (b.y/b.w + 1.0f) * 0.5f * height;
	c.x = (c.x/c.w + 1.0f) * 0.5f * width;
	c.y = (c.y/c.w + 1.0f) * 0.5f * height;

	DrawTriangle(data, width, height, 
		a, b, c);
}
