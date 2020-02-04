#ifndef PLANAR_REFLECTION_OBJ_LOADER
#define PLANAR_REFLECTION_OBJ_LOADER

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "material.h"

// OBJ file format reference:
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
class ObjModel
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	
	ObjModel(const std::string& file_path);
	ObjModel(const std::string& file_path, const Material& material);
	virtual ~ObjModel();

	const std::vector<glm::vec3>& GetVertices() const;
	const std::vector<uint32_t>& GetVertexIndices() const;
	const std::vector<glm::vec3>& GetNormals() const;
	const std::vector<uint32_t>& GetNormalIndices() const;
	const std::vector<glm::vec2>& GetUvs() const;
	const std::vector<uint32_t>& GetUvIndices() const;
	
	void LoadModel(const std::string& file_path);
	void UnloadModel();
	
	void Render() const;

	Material& GetMaterial();
	void SetMaterial(const Material& material);

	void SetLocalTransform(const glm::mat4& local_transform);
	void SetWorldTransform(const glm::mat4& world_transform);

	const glm::mat4& GetLocalTransform() const;
	const glm::mat4& GetWorldTransform() const;
	glm::mat4 GetModelTransform() const;
	
private:
	void CreateBuffers();
	void DestroyBuffers();
	
	void ParseVertexPosition(std::istream& line_stream);
	void ParseVertexNormal(std::istream& line_stream);
	void ParseVertexTexture(std::istream& line_stream);
	void ParseFace(std::istream& line_stream);

	static glm::vec3 Vec3FromStream(std::istream& line_stream);
	static glm::vec2 Vec2FromStream(std::istream& line_stream);
	static std::vector<Vertex> InterleaveData(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> uvs,
		std::vector<uint32_t> position_indices,
		std::vector<uint32_t> normal_indices,
		std::vector<uint32_t> uv_indices);
	
	std::vector<glm::vec3> positions_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> uvs_;
	std::vector<uint32_t> position_indices_;
	std::vector<uint32_t> normal_indices_;
	std::vector<uint32_t> uv_indices_;
	std::vector<Vertex> vertices_;

	GLuint vao_;
	GLuint vbo_;
	GLuint ibo_;

	glm::mat4 local_transform_;
	glm::mat4 world_transform_;

	Material material_;

	bool loaded_;
};

#endif