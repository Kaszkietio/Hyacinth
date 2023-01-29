#pragma once
#include <cstdint>
#include <Hyacinth/Scene.h>

namespace Hyacinth
{
	class Renderer
	{
	private:
		Scene m_scene{};

	public:
		Renderer();
		~Renderer();

		void OnUpdate(uint32_t data[], int32_t width, int32_t height);

	};
}
