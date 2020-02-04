#ifndef PLANAR_REFLECTION_MATERIAL
#define PLANAR_REFLECTION_MATERIAL

#include <glm/vec3.hpp>

class Material
{
public:
	Material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color);
	virtual ~Material();

	void SetAmbientColor(const glm::vec3& ambient_color);
	void SetDiffuseColor(const glm::vec3& diffuse_color);

	const glm::vec3& GetAmbientColor() const;
	const glm::vec3& GetDiffuseColor() const;

private:
	glm::vec3 ambient_color_;
	glm::vec3 diffuse_color_;
};

#endif