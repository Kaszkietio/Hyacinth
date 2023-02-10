#include <Lilia/Entity.h>

void Lilia::Entity::Draw(Framebuffer& fb, const glm::mat4& Proj, const glm::mat4& View)
{
	glm::mat4 Model = GetModelMatrix();

	for (auto& t : Triangles)
	{
	}
}
