#pragma once
#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Clipper.h>
#include <Lilia/Shading/ShadingModel.h>
#include <Lilia/Camera.h>

namespace Lilia
{
namespace VertexData
{
	//struct Input
	//{
	//	Triangle& Model;
	//	const glm::mat4& ModelMatrix;
	//	const glm::mat4& ProjectionViewMatrix;

	//	Input(
	//		Triangle& model,
	//		const glm::mat4& modelMatrix,
	//		const glm::mat4& projectionViewMatrix
	//	) : 
	//		Model(model),
	//		ModelMatrix(modelMatrix),
	//		ProjectionViewMatrix(projectionViewMatrix)
	//	{}
	//};

	//struct Payload
	//{
	//	Triangle World{};
	//	Triangle Clipping{};
	//};

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
		std::vector<Triangle> world;
		std::vector<Triangle> clipping;

		Payload(const uint64_t& vectorSize):
			world(vectorSize),
			clipping(vectorSize)
		{}
	};

}

namespace GeometryData
{
	//struct Input
	//{
	//	const Triangle& World;
	//	const Triangle& Clipping;
	//	Clipper& Clip;
	//	Input(const Triangle& world, const Triangle& clipping, Clipper& clipper):
	//		World(world),
	//		Clipping(clipping),
	//		Clip(clipper)
	//	{}
	//};

	//struct Payload
	//{
	//	Triangle World{};
	//	std::vector<Triangle> ClippedTriangles{};
	//};

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
		Payload(const uint64_t& vectorSize):
			world(vectorSize),
			clipping(vectorSize)
		{}
	};
}

namespace FragmentData
{
	struct Input
	{
		Framebuffer& Fb;
		const Triangle& World;
		const ScreenTriangle& WholeScreenTriangle;
		std::vector<ScreenTriangle>& ScreenTriangles;
		Texture& Tex;
		ShadingModel& Model;
		const std::vector<Light>& Lights;
		const glm::vec3& CameraPos;

		Input(
			Framebuffer& fb,
			const Triangle& world,
			const ScreenTriangle& wholeScreenTriangle,
			std::vector<ScreenTriangle>& screenTriangles,
			Texture& texture,
			ShadingModel& model,
			const std::vector<Light>& lights,
			const glm::vec3& cameraPos
		) :
			Fb(fb),
			World(world),
			WholeScreenTriangle(wholeScreenTriangle),
			ScreenTriangles(screenTriangles),
			Tex(texture),
			Model(model),
			Lights(lights),
			CameraPos(cameraPos)
		{}
	};

	//struct Input
	//{
	//	Framebuffer& fb;
	//	const std::vector<Triangle>& wholeWorld;
	//	std::vector<std::vector<Triangle>>& world;
	//	std::vector<std::vector<ScreenTriangle>>& screenTriangles;
	//	const std::vector<Texture>& texture;
	//	ShadingModel& shadingModel;
	//	const std::vector<Light>& lights;
	//	const Camera& camera;
	//};
}

}
