#pragma once

#include <Lilia/Payloads.h>

namespace Lilia
{
	class VertexShader
	{
	public:
		static VertexData::Payload Process(const VertexData::Input& input)
		{
			VertexData::Payload payload{};
			payload.World = input.Model.ApplyModelMatrix(input.ModelMatrix);
			payload.Clipping = payload.World.ApplyProjViewMatrix(input.ProjectionViewMatrix);

			return payload;
		}
	};
}
