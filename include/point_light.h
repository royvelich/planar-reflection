#ifndef PLANAR_REFLECTION_POINT_LIGHT
#define PLANAR_REFLECTION_POINT_LIGHT

#include <glm/vec3.hpp>

class PointLight
{
public:
	PointLight(const glm::vec3& position, const glm::vec3& ambient_light, const glm::vec3& diffuse_light);
	virtual ~PointLight();

	void SetPosition(const glm::vec3& position);
	void SetDiffuseLight(const glm::vec3& diffuse);
	void SetAmbientLight(const glm::vec3& ambient);
	
	const glm::vec3& GetPosition();
	const glm::vec3& GetDiffuseLight();
	const glm::vec3& GetAmbientLight();

private:
	glm::vec3 position_;
	glm::vec3 ambient_light_;
	glm::vec3 diffuse_light_;
};

#endif