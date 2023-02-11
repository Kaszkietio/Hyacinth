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
			std::vector<glm::vec4> vertice(t.vertice.size());

			for (int i = 0; i < t.vertice.size(); i++)
			{
				vertice[i] = t[i].vertex;
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
					tmp[j].vertex = vertice[index];
				}
				result[i] = tmp;
				result[i].state = Triangle::State::Clipping;
			}

			return result;
		}
	private:
		void SutherlandHodgman(std::vector<glm::vec4>& vertice) 
		{
			constexpr auto dist = [](const glm::vec4& v, int i) constexpr -> float
			{
				return v.w + ((i % 2 == 0 ? -1 : 1) * v[i / 2]);
			};

			std::vector<glm::vec4> tmp;
			int32_t indexA, indexB;

			// Create clipped polygon
			for (int border = 0; border < 6; border++)
			{
				tmp.clear();
				indexA = vertice.size() - 1;
				for(indexB = 0; indexB < vertice.size(); indexA = indexB++)
				{
					auto& A = vertice[indexA];
					auto& B = vertice[indexB];
					float distA = dist(A, border);
					float distB = dist(B, border);

					float signA = glm::sign(distA), signB = glm::sign(distB);

					// Start outside
					if (signA < 0)
					{
						// End outside
							// Nothing
						// End inside
						if(signB >= 0)
						{
							float distC = distA / (distA - distB);
							glm::vec4 C = A * (1 - distC) + B * distC;

							tmp.push_back(C);
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
							glm::vec4 C = A * (1 - distC) + B * distC;
							tmp.push_back(C);
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
