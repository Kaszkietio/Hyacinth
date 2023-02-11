#pragma once
#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Clipper.h>
#include <Lilia/Shading/ShadingModel.h>

namespace Lilia
{
namespace VertexData
{
	struct Input
	{
		Triangle& Model;
		const glm::mat4& ModelMatrix;
		const glm::mat4& ProjectionViewMatrix;

		Input(
			Triangle& model,
			const glm::mat4& modelMatrix,
			const glm::mat4& projectionViewMatrix
		) : 
			Model(model),
			ModelMatrix(modelMatrix),
			ProjectionViewMatrix(projectionViewMatrix)
		{}
	};

	struct Payload
	{
		Triangle World{};
		Triangle Clipping{};
	};
}

namespace GeometryData
{
	struct Input
	{
		const Triangle& World;
		const Triangle& Clipping;
		Clipper& Clip;
		Input(const Triangle& world, const Triangle& clipping, Clipper& clipper):
			World(world),
			Clipping(clipping),
			Clip(clipper)
		{}
	};

	struct Payload
	{
		Triangle World{};
		std::vector<Triangle> ClippedTriangles{};
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
}

}
