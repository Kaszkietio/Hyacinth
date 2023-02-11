#pragma once

#include <Lilia/Payloads.h>

namespace Lilia
{
	//class GeometryShader
	//{
	//public:
	//	static GeometryData::Payload Process(const GeometryData::Input& input)
	//	{
	//		GeometryData::Payload payload{};
	//		payload.World = input.World;
	//		Triangle dupa = input.Clipping;
	//		payload.ClippedTriangles = input.Clip.Clip(dupa);

	//		return payload;
	//	}
	//};
	struct GeometryShader
	{
		struct Input
		{
			const std::vector<Triangle>& world;
			const std::vector<Triangle>& clipping;
			const Clipper& clipper;

			Input(
				const std::vector<Triangle>& world,
				const std::vector<Triangle>& clipping,
				const Clipper& clipper
			) :
				world(world),
				clipping(clipping),
				clipper(clipper)
			{}
		};

		struct Payload
		{
			std::vector<std::vector<Triangle>> world;
			std::vector<std::vector<Triangle>> clipping;
		};
	private:
		Payload m_payload{};
	public:
		constexpr Payload& GetPayload() { return m_payload; }

		inline void Process(const Input& input)
		{
			uint64_t size = input.world.size();
			m_payload.world.clear(); m_payload.world.resize(size);
			m_payload.clipping.clear(); m_payload.clipping.resize(size);

			std::vector<Triangle> world{}, clipping{};
			for (uint64_t i = 0; i < size; i++)
			{
				input.clipper.Clip(input.clipping[i], input.world[i], clipping, world);
				m_payload.clipping[i] = clipping;
				m_payload.world[i] = world;
			}
		}
	};
}
