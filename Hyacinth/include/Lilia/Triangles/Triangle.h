#pragma once

#include <glm/glm.hpp>
#include <array>
#include <Lilia/Triangles/Vertex.h>
#include <Lilia/Triangles/ScreenTriangle.h>
#include <iostream>

#define assertm(exp, msg) assert(((void)msg, exp))

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

		constexpr const Vertex& operator[](int index) const
		{
			return vertice[index];
		}

		inline Triangle ApplyModelMatrix(const glm::mat4& Model) const
		{
			assertm(state == State::Model, "Applying model matrix to triangle not in model space!\n");

			Triangle result;
			glm::mat4 InverseTransposeModel = glm::transpose(glm::inverse(Model));
			for (int i = 0; i < vertice.size(); i++)
			{
				result.vertice[i].vertex = Model * vertice[i].vertex;
				result.vertice[i].normal = InverseTransposeModel * vertice[i].normal;
				result.vertice[i].normal.w = 0.0f;
			}
			result.state = State::World;

			return result;
		}

		inline Triangle ApplyProjViewMatrix(const glm::mat4& ProjViewMatrix) const
		{
			assertm(state == State::World, "Applying projview matrix to triangle not in world space!\n");

			Triangle result{};
			for (int i = 0; i < vertice.size(); i++)
			{
				result.vertice[i].vertex = ProjViewMatrix * vertice[i].vertex;
				result.vertice[i].normal = vertice[i].normal;
			}
			result.state = State::Clipping;
			return result;
		}

		inline Triangle ToNDC() const 
		{
			assertm(state == State::Clipping, "Converting triangle not in clipping space to NDC space!\n");

			Triangle result{};
			for (int i = 0; i < vertice.size(); i++)
			{
				result.vertice[i].vertex = vertice[i].vertex / vertice[i].vertex.w;
				result.vertice[i].vertex.w = vertice[i].vertex.w;
			}
			result.state = State::NDC;
			return result;
		}

		ScreenTriangle ToScreen(int32_t width, int32_t height) const
		{
			assertm(state == State::NDC, "Converting triangle not in NDC space to Screen space!\n");

			ScreenTriangle result{};
			for (int i = 0; i < vertice.size(); i++)
			{
				result[i].vertex = (vertice[i].vertex + 1.0f) * 0.5f;
				result[i].vertex.x *= width;
				result[i].vertex.y *= height;

				result[i].w = vertice[i].vertex.w;
			}

			return result;
		}
	};
}
