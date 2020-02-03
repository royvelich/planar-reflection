#ifndef PLANAR_REFLECTION_OBJ_LOADER
#define PLANAR_REFLECTION_OBJ_LOADER

#include <glm/glm.hpp>
#include <string>
#include <vector>

// OBJ file format reference:
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
class ObjModel
{
public:
	ObjModel(const std::string& file_path);
	virtual ~ObjModel();

	const std::vector<glm::vec3>& GetVertices() const;
	const std::vector<uint32_t>& GetVertexIndices() const;
	const std::vector<glm::vec3>& GetNormals() const;
	const std::vector<uint32_t>& GetNormalIndices() const;
	const std::vector<glm::vec2>& GetUvs() const;
	const std::vector<uint32_t>& GetUvIndices() const;
	void LoadModel(const std::string& file_path);

private:
	void ParseVertex(std::istream& line_stream);
	void ParseVertexNormal(std::istream& line_stream);
	void ParseVertexTexture(std::istream& line_stream);
	void ParseFace(std::istream& line_stream);
	
	glm::vec3 Vec3FromStream(std::istream& line_stream);
	glm::vec2 Vec2FromStream(std::istream& line_stream);
	
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> uvs_;
	std::vector<uint32_t> vertex_indices_;
	std::vector<uint32_t> normal_indices_;
	std::vector<uint32_t> uv_indices_;
};

#endif