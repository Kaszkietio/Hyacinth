#pragma once
#include <iostream>
#include <string_view>
#include <sstream>
#include <fstream>
#include "Entity.h"

namespace Hyacinth
{
	class ObjParser
	{
	private:

	public:
		static Entity Parse(std::string_view fileName, std::string entityName);
	};

	Entity ObjParser::Parse(std::string_view fileName, std::string entityName)
	{
		Entity result{};
		std::ifstream file;
		std::string s, type, tmp;
		std::stringstream stream;

		std::vector<glm::vec4> vertice, normals;
		std::vector<glm::vec2> texCoords;
		std::vector<Triangle> triangles;

		// Set name
		result.SetName(entityName);

		file.open(fileName.data());
		if (!file.is_open())
		{
			result.SetState(Entity::State::Invalid);
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

				normals.push_back(normal);
			}
			else if (type == "f")
			{
				Triangle triangle{};
				int64_t start = 0;
				int64_t end = 0;
				int32_t index;
				int32_t i = 0;

#ifndef FUN
				while (stream >> tmp)
				{
					// preset indexes in face to -1
					triangle.normalId[i] = triangle.textureCoordId[i] = triangle.vertexId[i] = -1;

					// vertex index
					end = tmp.find('/', start);
					if (end == std::string::npos)
					{
						index = std::stoi(tmp) - 1;
						triangle.vertexId[i] = index;
					}
					else
					{
						index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
						triangle.vertexId[i] = index;

						// texture coord index
						start = end + 1;
						end = tmp.find('/', start);
						if (end == start)
							triangle.textureCoordId[i] = -1;
						else if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.textureCoordId[i] = index;
						}

						// normal index
						start = end + 1;
						end = tmp.find('/', start);
						if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.normalId[i] = index;
						}
					}
					i++;
				}
				// Add face
				triangles.push_back(triangle);
#else
				while (stream >> tmp)
				{
					// vertex index
					end = tmp.find('/', start);
					if (end == std::string::npos)
					{
						index = std::stoi(tmp) - 1;
						triangle.vertice[i] = vertice[index];
					}
					else
					{
						index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
						triangle.vertice[i] = vertice[index];

						// texture coord index
						start = end + 1;
						end = tmp.find('/', start);
						if (end == start)
							triangle.texCoords[i] = glm::vec2(INFINITY);
						else if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.texCoords[i] = texCoords[index];
						}

						// normal index
						start = end + 1;
						end = tmp.find('/', start);
						if (end != std::string::npos)
						{
							index = std::stoi(tmp.substr(start, end - start + 1)) - 1;
							triangle.normals[i] = normals[index];
						}
					}
					i++;
				}
				// Add face
				triangles.push_back(triangle);

#endif
			}
		}

#ifndef FUN
		result.SetNormals(std::move(normals));
		result.SetVertice(std::move(vertice));
		result.SetTextureCoords(std::move(texCoords));
#endif

		result.SetMesh(std::move(triangles));
		result.SetState(Entity::State::Valid);

		file.close();
		return result;
	}
}
