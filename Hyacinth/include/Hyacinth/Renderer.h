#pragma once
#include <cstdint>

namespace Hyacinth
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void OnUpdate(uint32_t data[], int32_t width, int32_t height);

	};
}
