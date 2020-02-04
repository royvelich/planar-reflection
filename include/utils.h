#ifndef PLANAR_REFLECTION_UTILS
#define PLANAR_REFLECTION_UTILS
#include <string>
#include <glm/vec3.hpp>
#include <vector>

class Utils
{
public:
	static std::string TextFileToString(const std::string& file_path);
	static std::stringstream TextFileToStream(const std::string& file_path);
	static std::vector<glm::vec3> CalculateVertexNormals(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& vertex_indices);
};

#endif