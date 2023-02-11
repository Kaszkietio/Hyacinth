#include <Lilia/Scenes/TestScene.h>
#include <stbi_include.h>
#include <Lilia/Parser.h>

namespace Lilia
{
	TestScene::TestScene()
	{
		AddTextures();
		AddEntities();
		AddLights();
	}

	void TestScene::AddTextures()
	{
		static const std::string path = Scene::path + Scene::textureSuffix;

		// Add textures
		Texture red{ .Name = "RedTexture" };
		red.data = stbi_load((path + "RedTexture.jpg").c_str(), &red.width, &red.height, &red.channelCount, 4);
		Textures.push_back(red);

		Texture green{ .Name = "GreenTexture" };
		green.data = stbi_load((path + "GreenTexture.jpg").c_str(), &green.width, &green.height, &green.channelCount, 4);
		Textures.push_back(green);

		Texture blue{ .Name = "BlueTexture" };
		blue.data = stbi_load((path + "BlueTexture.jpg").c_str(), & blue.width, & blue.height, & blue.channelCount, 4);
		Textures.push_back(blue);

		Texture purple{ .Name = "PurpleTexture" };
		purple.data = stbi_load((path + "PurpleTexture.jpg").c_str(), &purple.width, &purple.height, &purple.channelCount, 4);
		Textures.push_back(purple);

		Texture smile{ .Name = "Smile" };
		smile.data = stbi_load("Smile.jpg",
			&smile.width,
			&smile.height,
			&smile.channelCount,
			4);
		Textures.push_back(smile);
	}
	void TestScene::AddEntities()
	{
		static const std::string path = Scene::path + Scene::objSuffix;

		// Add Entities
		Entity triangle = Parser::Parse(path + "triangle.obj", "triangle1");
		auto& t = Entities.emplace_back(triangle);
		t.TextureId = 0;
		t.UpdatePosition(glm::vec3(0.0f, 0.0f, -1.0f));
		t.UpdateScale(glm::vec3(2.0f));
		t.UpdateRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		t.UpdateModelMatrix();

		Entity triangle2 = Parser::Parse(path + "triangle.obj", "triangle2");
		auto& t2 = Entities.emplace_back(triangle2);
		t2.TextureId = 2;
		t2.UpdatePosition(glm::vec3(-2.0f, 0.0f, -2.0f));
		t2.UpdateScale(glm::vec3(2.0f));
		t2.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 315.0f);
		t2.UpdateModelMatrix();

		Entity cube = Parser::Parse(path + "cube.obj", "cube");
		auto& t4 = Entities.emplace_back(cube);
		t4.TextureId = 3;
		t4.UpdatePosition(glm::vec3(5.0f, 0.0f, -2.0f));
		t4.UpdateScale(glm::vec3(1.0f));
		t4.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t4.UpdateModelMatrix();

		Entity sphere = Parser::Parse(path + "sphere.obj", "sphere");
		auto& t5 = Entities.emplace_back(sphere);
		t5.TextureId = 3;
		t5.UpdatePosition(glm::vec3(4.0f, 2.0f, 8.0f));
		t5.UpdateScale(glm::vec3(1.0f));
		t5.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t5.UpdateModelMatrix();

		Entity smoothSphere = Parser::Parse(path + "smoothSphere.obj", "smoothSphere");
		auto& t8 = Entities.emplace_back(smoothSphere);
		t8.TextureId = 3;
		t8.UpdatePosition(glm::vec3(-3.0f, 2.0f, 8.0f));
		t8.UpdateScale(glm::vec3(1.0f));
		t8.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t8.UpdateModelMatrix();

		Entity plane = Parser::Parse(path + "plane.obj", "plane");
		auto& t6 = Entities.emplace_back(plane);
		t6.TextureId = 1;
		t6.UpdatePosition(glm::vec3(0.0f, -2.0f, 0.0f));
		t6.UpdateScale(glm::vec3(1.0f));
		t6.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
		t6.UpdateModelMatrix();

		//Entity triangle3 = Parser::Parse(path + "triangle.obj", "triangle3");
		//auto& t3 = Entities.emplace_back(triangle3);
		//t3.TextureId = 1;
		//t3.UpdatePosition(glm::vec3(0.0f, -3.0f, -50.0f));
		//t3.UpdateScale(glm::vec3(100.0f));
		//t3.UpdateRotation(glm::vec3(1.0f, 0.0f, 0.0f),  315.0f);
		//t3.UpdateModelMatrix();


		//Entity testplane = Parser::Parse(path + "testplane.obj", "testplane");
		//auto& t7 = Entities.emplace_back(testplane);
		//t7.TextureId = 3;
		//t7.UpdatePosition(glm::vec3(0.0f, 0.0f, -2.0f));
		//t7.UpdateScale(glm::vec3(1.0f));
		//t7.UpdateRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		//t7.UpdateModelMatrix();
	}
	void TestScene::AddLights()
	{
	}
}