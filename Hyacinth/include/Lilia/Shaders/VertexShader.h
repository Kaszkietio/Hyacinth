#pragma once

#include <Lilia/Payloads.h>

namespace Lilia
{
	//class VertexShader
	//{
	//public:
	//	static VertexData::Payload Process(const VertexData::Input& input)
	//	{
	//		VertexData::Payload payload{};
	//		payload.World = input.Model.ApplyModelMatrix(input.ModelMatrix);
	//		payload.Clipping = payload.World.ApplyProjViewMatrix(input.ProjectionViewMatrix);

	//		return payload;
	//	}
	//};
	struct VertexShader
	{
		struct Input
		{
			const std::vector<Triangle>& models;
			const glm::mat4& modelMatrix;
			const glm::mat4& viewMatrix;
			const glm::mat4& projMatrix;
			const glm::mat4& projViewMatrix;

			Input(
				const std::vector<Triangle>& models,
				const glm::mat4& modelMatrix,
				const glm::mat4& viewMatrix,
				const glm::mat4& projMatrix,
				const glm::mat4& projViewMatrix
			) :
				models(models),
				modelMatrix(modelMatrix),
				viewMatrix(viewMatrix),
				projMatrix(projMatrix),
				projViewMatrix(projViewMatrix)
			{}
		};
		struct Payload
		{
			std::vector<Triangle> world{};
			std::vector<Triangle> clipping{};
		};
	private:
		Payload m_payload{};
	public:
		constexpr Payload& GetPayload() { return m_payload; };

		inline void Process(const Input& input)
		{
			uint64_t size = input.models.size();
			m_payload.world.clear(); m_payload.world.resize(size);
			m_payload.clipping.clear(); m_payload.clipping.resize(size);

			for (uint64_t i = 0; i < size; ++i)
			{
				m_payload.world[i] = input.models[i].ApplyModelMatrix(input.modelMatrix);
				m_payload.clipping[i] = m_payload.world[i].ApplyProjViewMatrix(input.projViewMatrix);
			}
		}
	};
}
