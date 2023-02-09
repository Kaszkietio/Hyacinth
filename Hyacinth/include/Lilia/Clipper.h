#pragma once
#include <vector>
#include <Lilia/Triangles/Triangle.h>
#include <Lilia/Triangles/ScreenTriangle.h>

namespace Lilia
{
	class Clipper
	{
	public:
		virtual std::vector<Triangle> Clip(Triangle& t) =0;
	private:
	};

	class SutherlandHodgmanClipper : public Clipper
	{
	public:
		std::vector<Triangle> Clip(Triangle& t) override
		{
			std::vector<Vertex> vertice(t.vertice.size());

			for (int i = 0; i < t.vertice.size(); i++)
			{
				vertice[i] = t[i];
			}

			// Apply Sutherland-Hodgman
			SutherlandHodgman(vertice);
			// We have current polygon, gotta create triangles out of it
			if (vertice.size() == 0) return {};
			std::vector<Triangle> result(std::max(vertice.size() - 2, (uint64_t)0));

			for (int i = 0; i < vertice.size() - 2; i++)
			{
				Triangle tmp{};
				for (int j = 0; j < tmp.vertice.size(); j++)
				{
					int index = j != 0 ? i + j : 0;
					//tmp.vertice[j] = vertice[index].vertex;
					//tmp.normals[j] = vertice[index].normal;
					//tmp.texCoords[j] = vertice[index].texCoord;
					tmp[j] = vertice[index];
				}
				result[i] = tmp;
			}

			return result;
		}
	private:
		void SutherlandHodgman(std::vector<Vertex>& vertice) 
		{
			constexpr auto dist = [](const glm::vec4& v, int i) -> float
			{
				float tmp = (i % 2 == 0 ? -1 : 1) * v[i / 2];
				float res = v.w + tmp;
				return res;
			};

			std::vector<Vertex> tmp;

			// Create clipped polygon
			for (int border = 0; border < 6; border++)
			{
				tmp.clear();
				int index = vertice.size() - 1;
				for (int i = 0; i < vertice.size(); i++, index++)
				{
					auto& A = vertice[index % vertice.size()];
					auto& B = vertice[(index + 1) % vertice.size()];
					float distA = dist(A.vertex, border);
					float distB = dist(B.vertex, border);

					float signA = glm::sign(distA), signB = glm::sign(distB);

					// Start outside
					if (signA < 0)
					{
						// End outside
						if (signB < 0)
						{
							// Nothing
						}
						// End inside
						else
						{
							float distC = distA / (distA - distB);
							//glm::vec4 vC = ((A.vertex * (1 - distC) / A.vertex.w) + (B.vertex * distC / B.vertex.w)) / ;
							glm::vec4 vC = A.vertex * (1 - distC) + B.vertex * distC;
							glm::vec4 nC = glm::normalize(A.normal * (1 - distC) + B.normal * distC);
							glm::vec2 tC = A.texCoord * (1 - distC) + B.texCoord * distC;

							//glm::vec4 vC = lerp_w(A.vertex, B.vertex, A.vertex.w, B.vertex.w, distC);
							//glm::vec4 nC = lerp_w(A.normal, B.normal, A.vertex.w, B.vertex.w, distC);
							//glm::vec2 tC = lerp_w(A.texCoord, B.texCoord, A.vertex.w, B.vertex.w, distC);

							tmp.push_back(Vertex{ .vertex = vC, .normal = nC, .texCoord = tC });
							tmp.push_back(B);
						}
					}
					// Start inside
					else
					{
						// End outside
						if (signB < 0)
						{
							float distC = distA / (distA - distB);
							glm::vec4 vC = A.vertex * (1 - distC) + B.vertex * distC;
							glm::vec4 nC = glm::normalize(A.normal * (1 - distC) + B.normal * distC);
							glm::vec2 tC = A.texCoord * (1 - distC) + B.texCoord * distC;

							//glm::vec4 vC = lerp_w(A.vertex, B.vertex, A.vertex.w, B.vertex.w, distC);
							//glm::vec4 nC = lerp_w(A.normal, B.normal, A.vertex.w, B.vertex.w, distC);
							//glm::vec2 tC = lerp_w(A.texCoord, B.texCoord, A.vertex.w, B.vertex.w, distC);

							tmp.push_back(Vertex{ .vertex = vC, .normal = nC, .texCoord = tC });
						}
						// End inside
						else
						{
							tmp.push_back(B);
						}
					}
				}

				// copy tmp to vertice
				vertice = tmp;
			}
		}

	};
}
