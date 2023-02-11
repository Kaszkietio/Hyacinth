#pragma once
#include <Lilia/Scenes/Scene.h>

namespace Lilia
{
	class TestScene : public Scene
	{
	public:
		TestScene();
	private:
		void AddTextures();
		void AddEntities();
		void AddLights();
	};
}
