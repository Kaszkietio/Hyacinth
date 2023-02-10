#pragma once

#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Light.h>

namespace Lilia
{
	class ShadingModel
	{
	public:
		Triangle T;

		virtual void Update(
			Triangle& t, 
			const std::vector<Light>& lights,
			const glm::vec3& cameraPos
		) = 0;

		virtual glm::vec3 GetColor(
			Triangle& triangle, 
			Vertex& v, 
			const std::vector<Light>& lights, 
			Texture& texture,
			const glm::vec3& cameraPos
		) = 0;

		glm::vec3 PhongEquation(
			Vertex& v, 
			const std::vector<Light>& lights,
			const glm::vec3& CameraPos
		)
		{
			constexpr float ka = 0.1f;
			constexpr float kd = 0.6f;
			constexpr float ks = 0.3f;
			constexpr glm::vec3 LightPos(0.0f, 0.0f, 0.0f);
			constexpr glm::vec3 LightCol(1.0f);

			// ambient
			glm::vec3 ambient = ka * LightCol;

			// diffuse
			glm::vec3 vertex = v.vertex;
			glm::vec3 lightDir = glm::normalize(LightPos - vertex);
			glm::vec3 normal = v.normal;
			glm::vec3 diffuse = LightCol * glm::dot(normal, lightDir) * kd;
			
			// specular
			glm::vec3 V = glm::normalize(CameraPos - vertex);
			glm::vec3 reflect = glm::normalize(glm::reflect(lightDir, normal));
			glm::vec3 specular = LightCol * ks * powf(glm::dot(reflect, V), 32.0f);

			auto t = (ambient + diffuse + specular);
			auto xd = t * glm::vec3(1.0f, 0.0f, 0.0f);
			return glm::clamp(xd, glm::vec3(0.0f), glm::vec3(1.0f));

		}
	};

	class ConstantShadingModel : public ShadingModel
	{
	public:
		Vertex cur{};
		glm::vec3 col{};

		void Update(
			Triangle& t,
			const std::vector<Light>& lights,
			const glm::vec3& cameraPos
		) override
		{
			T = t;
			cur = Vertex::bar(T[0], T[1], T[2], glm::vec3(1.0f / 3));
			col = PhongEquation(cur, lights, cameraPos);
		}

		glm::vec3 GetColor(
			Triangle & triangle, 
			Vertex & v, 
			const std::vector<Light>&lights, 
			Texture & texture,
			const glm::vec3& CameraPos
		) override
		{
			return col;
		}

	};

	class PhongShadingModel : public ShadingModel
	{
	public:
		Vertex cur{};
		glm::vec3 col{};

		void Update(
			Triangle& t,
			const std::vector<Light>& lights,
			const glm::vec3& cameraPos
		) override
		{
			T = t;
		}

		glm::vec3 GetColor(
			Triangle & triangle, 
			Vertex & v, 
			const std::vector<Light>&lights, 
			Texture & texture,
			const glm::vec3& CameraPos
		) override
		{
			return PhongEquation(v, lights, CameraPos);
		}

	};
}
