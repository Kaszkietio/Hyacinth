#pragma once

#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Light.h>

namespace Lilia
{
	class ShadingModel
	{
	protected:
		static constexpr glm::vec3 LightPos = glm::vec3(0.0f, 2.0f, 8.0f);
		static constexpr glm::vec3 LightCol = glm::vec3(1.0f);
		static constexpr glm::vec3 FogColor = glm::vec3(0.5f);
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

			// ambient
			glm::vec3 ambient = ka * LightCol;

			// diffuse
			glm::vec3 vertex = v.vertex;

			glm::vec3 lightVertexVec = LightPos - vertex;
			glm::vec3 lightDir = glm::normalize(lightVertexVec);
			glm::vec3 normal = v.normal;
			glm::vec3 diffuse = LightCol * glm::dot(normal, lightDir) * kd;

			// specular
			glm::vec3 V = glm::normalize(CameraPos - vertex);
			glm::vec3 reflect = glm::normalize(glm::reflect(lightDir, normal));
			glm::vec3 specular = LightCol * ks * powf(glm::dot(reflect, V), 32.0f);

			float distSq = glm::dot(lightVertexVec, lightVertexVec);

			float lightDistFactor = 3.0f / distSq;

			auto t = (ambient + (diffuse + specular) * lightDistFactor);
			auto xd = glm::clamp(
				t * texture.GetPixelVector(0.0f, 0.0f),
				glm::vec3(0.0f), glm::vec3(1.0f)
			);
			return xd;
		}

		float getFogFactor(float fogCoordinate)
		{
			float result = 0.0f;
			constexpr float ls = 5000.0f;
			constexpr float le = 10000.0f;

			constexpr float fogLength = le - ls;
			result = (le - fogCoordinate) / fogLength;

			return 1.0f - glm::clamp(result, 0.0f, 1.0f);
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
			glm::vec3 fogDist = glm::vec3(v.vertex) - CameraPos;
			float fogDistSq = glm::dot(fogDist, fogDist);
			float fogFactor = getFogFactor(fogDistSq);
			return glm::mix(col, FogColor, fogFactor);
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
			auto objColor = ApplyBaricentric();
			glm::vec3 fogDist = glm::vec3(v.vertex) - CameraPos;
			float fogFactor = getFogFactor(glm::dot(fogDist, fogDist));
			return glm::mix(objColor, FogColor, fogFactor);
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

		inline glm::vec3 ApplyBaricentric()
		{
			return glm::vec3(
				col[0].x * result.x + col[1].x * result.y + col[2].x * result.z,
				col[0].y * result.x + col[1].y * result.y + col[2].y * result.z,
				col[0].z * result.x + col[1].z * result.y + col[2].z * result.z
			);
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
			auto objColor = PhongEquation(v, lights, texture, CameraPos);
			glm::vec3 fogDist = glm::vec3(v.vertex) - CameraPos;
			float fogFactor = getFogFactor(glm::dot(fogDist, fogDist));
			return glm::mix(objColor, FogColor, fogFactor);
		}

	};
}
