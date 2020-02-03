#include "obj_model.h"
#include "utils.h"
#include <istream>
#include <iostream>
#include <sstream>

ObjModel::ObjModel(const std::string& file_path)
{
	LoadModel(file_path);
}

ObjModel::~ObjModel()
{
	
}

const std::vector<glm::vec3>& ObjModel::GetVertices() const
{
	return vertices_;
}

const std::vector<uint32_t>& ObjModel::GetVertexIndices() const
{
	return vertex_indices_;
}

const std::vector<glm::vec3>& ObjModel::GetNormals() const
{
	return normals_;
}

const std::vector<uint32_t>& ObjModel::GetNormalIndices() const
{
	return normal_indices_;
}

const std::vector<glm::vec2>& ObjModel::GetUvs() const
{
	return uvs_;
}

const std::vector<uint32_t>& ObjModel::GetUvIndices() const
{
	return uv_indices_;
}

void ObjModel::ParseVertex(std::istream& line_stream)
{
	vertices_.push_back(Vec3FromStream(line_stream));
}

void ObjModel::ParseVertexNormal(std::istream& line_stream)
{
	normals_.push_back(Vec3FromStream(line_stream));
}

void ObjModel::ParseVertexTexture(std::istream& line_stream)
{
	uvs_.push_back(Vec2FromStream(line_stream));
}

void ObjModel::ParseFace(std::istream& line_stream)
{
	char back_slash;
	for (int i = 0; i < 3; i++)
	{
		uint32_t vertex_index;
		uint32_t normal_index;
		uint32_t uv_index;

		// Read the next vertex index
		line_stream >> std::ws >> vertex_index >> std::ws;
		vertex_indices_.push_back(vertex_index);

		// Continue if next char is not a back-slash, since normal/texture indices do not exist
		if (line_stream.peek() != '/')
		{
			continue;
		}

		// Otherwise, consume the next back-slash
		line_stream >> back_slash >> std::ws;
		if (line_stream.peek() == '/')
		{
			// If we had two consecutive back-slashes, then only vertex/normal indices exist
			// Therefore, read the next normal index, and continue
			line_stream >> back_slash >> std::ws >> normal_index;
			normal_indices_.push_back(vertex_index);
			continue;
		}

		// Read the next uv index
		line_stream >> uv_index;
		uv_indices_.push_back(uv_index);

		// If no additional back-slash follows, then only vertex/texture indices exist
		if (line_stream.peek() != '/')
		{
			continue;
		}

		// Otherwise, read the next back-slash, and then read the next normal index
		line_stream >> back_slash >> normal_index;
		normal_indices_.push_back(vertex_index);
	}
}

void ObjModel::LoadModel(const std::string& file_path)
{
	auto ss_file = Utils::TextFileToStream(file_path);

	// While we haven't reached the end of file
	while (!ss_file.eof())
	{
		// Read the next line
		std::string current_line;
		std::getline(ss_file, current_line);
		std::istringstream ss_line(current_line);
		
		// Read the line type (single character)
		std::string line_type;
		ss_line >> std::ws >> line_type;

		// Parse the OBJ file data based on the line type
		if (line_type == "v")
		{
			ParseVertex(ss_line);
		}
		else if (line_type == "vn")
		{
			ParseVertexNormal(ss_line);
		}
		else if (line_type == "vt")
		{
			ParseVertexTexture(ss_line);
		}
		else if (line_type == "f")
		{
			ParseFace(ss_line);
		}
		else if (line_type == "#" || line_type == "")
		{
			// Comment or an empty line
		}
		else
		{
			std::cerr << "Cannot parse OBJ line: " << current_line << std::endl;
		}
	}
}

glm::vec3 ObjModel::Vec3FromStream(std::istream& line_stream)
{
	float x, y, z;
	line_stream >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 ObjModel::Vec2FromStream(std::istream& line_stream)
{
	float x, y;
	line_stream >> x >> std::ws >> y;
	return glm::vec2(x, y);
}