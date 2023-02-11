#pragma once

#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Light.h>

namespace Lilia
{
	class ShadingModel
	{
	public:
		std::string Name{};
		Triangle T;

		virtual void Update(
			const Triangle& t, 
			const std::vector<Light>& lights,
			Texture& texture,
			const glm::vec3& cameraPos
		) = 0;

		virtual glm::vec3 GetColor(
			const Triangle& triangle, 
			Vertex& v, 
			const std::vector<Light>& lights, 
			Texture& texture,
			const glm::vec3& cameraPos
		) = 0;

		glm::vec3 PhongEquation(
			Vertex& v, 
			const std::vector<Light>& lights,
			Texture& texture,
			const glm::vec3& CameraPos
		)
		{
			constexpr float ka = 0.1f;
			constexpr float kd = 0.6f;
			constexpr float ks = 0.3f;
			constexpr glm::vec3 LightPos(0.0f, 2.0f, 8.0f);
			constexpr glm::vec3 LightCol(1.0f);

			// ambient
			glm::vec3 ambient = ka * LightCol;

			// diffuse
			glm::vec3 vertex = v.vertex;

			glm::vec3 lightVertex = LightPos - vertex;
			glm::vec3 lightDir = glm::normalize(lightVertex);
			glm::vec3 normal = v.normal;
			glm::vec3 diffuse = LightCol * glm::dot(normal, lightDir) * kd;
			
			// specular
			glm::vec3 V = glm::normalize(CameraPos - vertex);
			glm::vec3 reflect = glm::normalize(glm::reflect(lightDir, normal));
			glm::vec3 specular = LightCol * ks * powf(glm::dot(reflect, V), 32.0f);

			float distFactor = 1.5f / glm::dot(lightVertex, lightVertex);

			auto t = (ambient + (diffuse + specular) * distFactor);
			auto xd = t * texture.GetPixelVector(0.0f, 0.0f);
			return glm::clamp(xd, glm::vec3(0.0f), glm::vec3(1.0f));

		}
	};

	class ConstantShadingModel : public ShadingModel
	{
	public:
		Vertex cur{};
		glm::vec3 col{};
		ConstantShadingModel() { Name = "Constant"; }

		void Update(
			const Triangle& t,
			const std::vector<Light>& lights,
			Texture& texture,
			const glm::vec3& cameraPos
		) override
		{
			T = t;
			cur = Vertex::bar(T[0], T[1], T[2], glm::vec3(1.0f / 3));
			col = PhongEquation(cur, lights, texture, cameraPos);
		}

		glm::vec3 GetColor(
			const Triangle & triangle, 
			Vertex & v, 
			const std::vector<Light>&lights, 
			Texture & texture,
			const glm::vec3& CameraPos
		) override
		{
			return col;
		}

	};

	class GouraudShadingModel : public ShadingModel
	{
	public:
		std::array<glm::vec3,3> col{};
		glm::vec3 result{};
		GouraudShadingModel() { Name = "Gouraud"; }

		void Update(
			const Triangle& t,
			const std::vector<Light>& lights,
			Texture& texture,
			const glm::vec3& cameraPos
		) override
		{
			T = t;
			col[0] = PhongEquation(T[0], lights, texture, cameraPos);
			col[1] = PhongEquation(T[1], lights, texture, cameraPos);
			col[2] = PhongEquation(T[2], lights, texture, cameraPos);
		}

		glm::vec3 GetColor(
			const Triangle & triangle, 
			Vertex & v, 
			const std::vector<Light>&lights, 
			Texture & texture,
			const glm::vec3& CameraPos
		) override
		{
			Barycentric(v.vertex);
			return result.x * col[0] + result.y * col[1] + result.z * col[2];
		}

		void Barycentric(
			const glm::vec4& p
		)
		{
			glm::vec4 v0 = T[1].vertex - T[0].vertex, v1 = T[2].vertex - T[0].vertex, v2 = p - T[0].vertex;
			float d00 = glm::dot(v0, v0);
			float d01 = glm::dot(v0, v1);
			float d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0);
			float d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			result.y = (d11 * d20 - d01 * d21) / denom;
			result.z = (d00 * d21 - d01 * d20) / denom;
			result.x = 1.0f - result.y - result.z;
		}

	};

	class PhongShadingModel : public ShadingModel
	{
	public:
		Vertex cur{};
		glm::vec3 col{};
		PhongShadingModel() { Name = "Phong"; }

		void Update(
			const Triangle& t,
			const std::vector<Light>& lights,
			Texture& texture,
			const glm::vec3& cameraPos
		) override
		{
			T = t;
		}

		glm::vec3 GetColor(
			const Triangle & triangle, 
			Vertex & v, 
			const std::vector<Light>&lights, 
			Texture & texture,
			const glm::vec3& CameraPos
		) override
		{
			return PhongEquation(v, lights, texture, CameraPos);
		}

	};
}
