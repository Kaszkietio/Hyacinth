#pragma once

#include <Lilia/Payloads.h>

namespace Lilia
{
	class GeometryShader
	{
	public:
		static GeometryData::Payload Process(const GeometryData::Input& input)
		{
			GeometryData::Payload payload{};
			payload.World = input.World;
			Triangle dupa = input.Clipping;
			payload.ClippedTriangles = input.Clip.Clip(dupa);

			return payload;
		}
	};
}
