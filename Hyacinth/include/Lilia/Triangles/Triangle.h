#pragma once

#include <glm/glm.hpp>
#include <array>
#include <Lilia/Triangles/ScreenTriangle.h>
#include <Lilia/Triangles/Vertex.h>

namespace Lilia
{

	struct Triangle
	{
		enum class State
		{
			Model, World, View, Clipping, NDC
		};

		State state{ State::Model };
		std::array<Vertex, 3> vertice{};

		constexpr Vertex& operator[](int index)
		{
			return vertice[index];
		}

		Triangle ApplyModelMatrix(const glm::mat4& Model)
		{
			Triangle result;
			glm::mat4 InverseTransposeModel = glm::inverse(glm::transpose(Model));
			for (int i = 0; i < vertice.size(); i++)
			{
				result.vertice[i].vertex = Model * vertice[i].vertex;
				result.vertice[i].normal = glm::normalize(InverseTransposeModel * vertice[i].normal);
			}
			result.state = State::World;

			return result;
		}

		void ApplyProjViewMatrix(const glm::mat4& Proj, const glm::mat4& View)
		{
			for (int i = 0; i < vertice.size(); i++)
			{
				vertice[i].vertex = Proj * View * vertice[i].vertex;
			}
			state = State::Clipping;
		}

		void ToNDC()
		{
			for (int i = 0; i < vertice.size(); i++)
			{
				vertice[i].vertex /= vertice[i].vertex.w;
			}
			state = State::NDC;
		}

		ScreenTriangle ToScreen(int32_t width, int32_t height, Texture& texture)
		{
			ScreenTriangle result{ .texture = texture };
			for (int i = 0; i < vertice.size(); i++)
			{
				result[i].vertex = (vertice[i].vertex + 1.0f) * 0.5f;
				result[i].vertex.x *= width;
				result[i].vertex.y *= height;

				result[i].normal = vertice[i].normal;
				result[i].texCoord = vertice[i].texCoord;
			}

			return result;
		}
	};
}
