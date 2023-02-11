#pragma once

#include <iostream>
#include <string_view>
#include <sstream>
#include <fstream>
#include <Lilia/Entity.h>

namespace Lilia::Parser
{
	static Entity Parse(std::string_view fileName, std::string entityName)
	{
		Entity result{};
		std::ifstream file;
		std::string s, type, tmp;
		std::stringstream stream;

		std::vector<glm::vec4> vertice, normals;
		std::vector<glm::vec2> texCoords;
		std::vector<Triangle> triangles;

		// Set name
		result.Name = entityName;

		file.open(fileName.data());
		if (!file.is_open())
		{
			result.State = Entity::State::Invalid;
			return result;
		}

		while (std::getline(file, s))
		{
			stream.clear();
			stream.str(s);

			stream >> type;

			if (type == "v")
			{
				glm::vec4 vertex(1.0f);

				stream >> tmp;
				vertex.x = std::stof(tmp);
				stream >> tmp;
				vertex.y = std::stof(tmp);
				stream >> tmp;
				vertex.z = std::stof(tmp);

				vertice.push_back(vertex);
			}
			else if (type == "vt")
			{
				glm::vec2 texCoord{};
				
				stream >> tmp;
				texCoord.x = std::stof(tmp);
				stream >> tmp;
				texCoord.y = std::stof(tmp);

				texCoords.push_back(texCoord);
			}
			else if (type == "vn")
			{
				glm::vec4 normal(0.0f);

				stream >> tmp;
				normal.x = std::stof(tmp);
				stream >> tmp;
				normal.y = std::stof(tmp);
				stream >> tmp;
				normal.z = std::stof(tmp);

				normals.push_back(glm::normalize(normal));
			}
			else if (type == "f")
			{
				Triangle triangle{};
				int64_t start = 0;
				int64_t end = 0;
				int32_t index;
				int32_t i = 0;

				while (stream >> tmp)
				{
					if (i == 3)
					{
						Triangle triangle2{};
						triangle2[0] = triangle[2];
						triangle2[1] = triangle[0];

						start = 0;
						// vertex index
						end = tmp.find('/', start);
						if (end == std::string::npos)
						{
							index = std::stoi(tmp) - 1;
							triangle2[2].vertex = vertice[index];
						}
						else
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle2[2].vertex = vertice[index];

							// texture coord index
							start = end + 1;
							end = tmp.find('/', start);
							if (end == start)
								triangle2[2].texCoord = glm::vec2(INFINITY);
							else if (end != std::string::npos)
							{
								index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
								triangle2[2].texCoord = texCoords[index];
							}

							// normal index
							start = end + 1;
							end = tmp.length();
							if (end != std::string::npos)
							{
								index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
								triangle2[2].normal = normals[index];
							}
						}

						triangles.push_back(triangle2);
						break;
					}

					start = 0;
					// vertex index
					end = tmp.find('/', start);
					if (end == std::string::npos)
					{
						index = std::stoi(tmp) - 1;
						triangle.vertice[i].vertex = vertice[index];
					}
					else
					{
						index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
						triangle.vertice[i].vertex = vertice[index];

						// texture coord index
						start = end + 1;
						end = tmp.find('/', start);
						if (end == start)
							triangle.vertice[i].texCoord = glm::vec2(INFINITY);
						else if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.vertice[i].texCoord = texCoords[index];
						}

						// normal index
						start = end + 1;
						end = tmp.length();
						if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.vertice[i].normal = normals[index];
						}
					}
					i++;
				}
				// Add face
				triangles.push_back(triangle);
			}
		}

		result.Triangles = std::move(triangles);
		result.State = Entity::State::Valid;

		file.close();
		return result;
	}
}
