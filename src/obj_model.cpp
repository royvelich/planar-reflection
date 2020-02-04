#include "obj_model.h"
#include "utils.h"
#include <istream>
#include <iostream>
#include <sstream>

ObjModel::ObjModel(const std::string& file_path) :
	vao_(0),
	vbo_vertices_(0),
	ibo_vertices_(0),
	vbo_normals_(0),
	ibo_normals_(0),
	loaded_(false),
	material_(glm::vec3(1,1,1), glm::vec3(1,1,1)),
	world_transform_(glm::mat4(1.0)),
	local_transform_(glm::mat4(1.0))
{
	LoadModel(file_path);
}

ObjModel::ObjModel(const std::string& file_path, const Material& material) :
	ObjModel(file_path)
{
	SetMaterial(material);
}

ObjModel::~ObjModel()
{
	DestroyBuffers();
}

void ObjModel::CreateBuffers()
{
	// Cleanup previous allocated buffers
	DestroyBuffers();

	// Create VAO (vertex array object) and assign attributes
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glBindVertexArray(0);

	// Create vertex and normal data attributes
	CreateAttribute(0, vbo_vertices_, ibo_vertices_, vertices_, vertex_indices_);
	CreateAttribute(1, vbo_normals_, ibo_normals_, normals_, normal_indices_);
}

void ObjModel::DestroyBuffers()
{
	// Destroy vertex and normal data attributes
	DestroyAttribute(vbo_vertices_, ibo_vertices_);
	DestroyAttribute(vbo_normals_, ibo_normals_);
	
	// Destroy VAO
	if (vao_ != 0)
	{
		glDeleteVertexArrays(1, &vao_);
		vao_ = 0;
	}

	loaded_ = false;
}

void ObjModel::CreateAttribute(GLuint id, GLuint& vbo, GLuint& ibo, const std::vector<glm::vec3>& data, const std::vector<uint32_t>& indices)
{
	// Create VBO (vertex buffer object) on GPU, and copy vertex data from CPU
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);
	glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(id);

	// Create IBO (index buffer object)
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
}

void ObjModel::DestroyAttribute(GLuint& vbo, GLuint& ibo)
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (ibo != 0)
	{
		glDeleteBuffers(1, &ibo);
		ibo = 0;
	}
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

	// Estimate vertex normals in case they were not provided in the OBJ file
	if(normals_.empty())
	{
		normals_ = Utils::CalculateVertexNormals(vertices_, vertex_indices_);
		normal_indices_ = vertex_indices_;
	}

	// Initialize buffers on GPU with newly loaded model
	CreateBuffers();

	// Model successfully loaded
	loaded_ = true;
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

void ObjModel::UnloadModel()
{
	if(loaded_)
	{
		DestroyBuffers();
		vertices_.clear();
		normals_.clear();
		vertex_indices_.clear();
		normal_indices_.clear();
		loaded_ = false;
	}
}

void ObjModel::Render() const
{
	if (loaded_)
	{
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
		glBindVertexArray(0);
	}
}

const Material& ObjModel::GetMaterial() const
{
	return material_;
}

void ObjModel::SetMaterial(const Material& material)
{
	material_ = material;
}

void ObjModel::SetLocalTransform(const glm::mat4& local_transform)
{
	local_transform_ = local_transform;
}

void ObjModel::SetWorldTransform(const glm::mat4& world_transform)
{
	world_transform_ = world_transform;
}

const glm::mat4& ObjModel::GetLocalTransform() const
{
	return local_transform_;
}

const glm::mat4& ObjModel::GetWorldTransform() const
{
	return world_transform_;
}

const glm::mat4& ObjModel::GetModelTransform() const
{
	return world_transform_ * local_transform_;
}