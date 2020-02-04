#include "utils.h"
#include <ostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

std::string Utils::TextFileToString(const std::string& file_path)
{
	return TextFileToStream(file_path).str();
}

std::stringstream Utils::TextFileToStream(const std::string& file_path)
{
	std::stringstream ss;
	std::ifstream fs;
	fs.open(file_path, std::ios::in);

	if (!fs.fail())
	{
		ss << fs.rdbuf();
	}
	else
	{
		std::cerr << "Error reading text file: " << file_path << std::endl;
	}

	fs.close();
	
	return ss;
}

std::vector<glm::vec3> Utils::CalculateVertexNormals(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& vertex_indices)
{
	std::vector<glm::vec3> normals(vertices.size());
	std::vector<uint32_t> adjacent_faces_count(vertices.size());

	for (size_t i = 0; i < adjacent_faces_count.size(); i++)
	{
		adjacent_faces_count[i] = 0;
	}

	for (size_t i = 0; i < vertex_indices.size(); i += 3)
	{
		const uint32_t v1_index = vertex_indices[i];
		const uint32_t v2_index = vertex_indices[i + 1];
		const uint32_t v3_index = vertex_indices[i + 2];
		
		glm::vec3 v0 = vertices.at(v1_index);
		glm::vec3 v1 = vertices.at(v2_index);
		glm::vec3 v2 = vertices.at(v3_index);

		glm::vec3 u = v0 - v1;
		glm::vec3 v = v2 - v1;
		const glm::vec3 normal = glm::normalize(-glm::cross(u, v));

		normals.at(v1_index) += normal;
		normals.at(v2_index) += normal;
		normals.at(v3_index) += normal;

		adjacent_faces_count.at(v1_index) += 1;
		adjacent_faces_count.at(v2_index) += 1;
		adjacent_faces_count.at(v3_index) += 1;
	}

	for (size_t i = 0; i < normals.size(); i++)
	{
		normals[i] /= adjacent_faces_count[i];
		normals[i] = glm::normalize(normals[i]);
	}

	return normals;
}