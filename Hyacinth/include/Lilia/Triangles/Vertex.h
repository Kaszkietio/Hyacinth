#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

namespace Lilia
{
	struct Vertex
	{
		glm::vec4 vertex{};
		glm::vec4 normal{};
		glm::vec2 texCoord{};

		static inline Vertex lerp(const Vertex& a, const Vertex& b, float q)
		{
			Vertex result;
			result.vertex = glm::lerp(a.vertex, b.vertex, glm::vec4(q));
			result.normal = glm::normalize(glm::lerp(a.normal, b.normal, glm::vec4(q)));
			result.texCoord = glm::lerp(a.texCoord, b.texCoord, glm::vec2(q));
			return result;
		}

		static inline Vertex lerpW(const Vertex& a, const Vertex& b, float q, float w1, float w2)
		{
			Vertex result;
			float q1 = q / w1;
			float q2 = (1.0f - q) / w2;
			float denom = q1 + q2;
			result.vertex = (a.vertex * q1 + b.vertex * q2) / denom;
			result.normal = glm::normalize((a.normal * q1 + b.normal * q2) / denom);
			result.texCoord = (a.texCoord * q1 + b.texCoord * q2) / denom;
			return result;
		}

		static inline Vertex bar(const Vertex& a, const Vertex& b, const Vertex& c, const glm::vec3& q)
		{
			Vertex result;
			result.vertex = a.vertex * q.x + b.vertex * q.y + c.vertex * q.z;
			result.normal = a.normal * q.x + b.normal * q.y + c.normal * q.z;
			result.texCoord = a.texCoord * q.x + b.texCoord * q.y + c.texCoord * q.z;
			return result;
		}
	};
}
