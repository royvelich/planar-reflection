#include "point_light.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse) :
	position_(position),
	diffuse_light_(diffuse),
	ambient_light_(ambient)
{
	
}

PointLight::~PointLight()
{
	
}

void PointLight::SetPosition(const glm::vec3& position)
{
	position_ = position;
}

void PointLight::SetDiffuseLight(const glm::vec3& diffuse)
{
	diffuse_light_ = diffuse;
}

void PointLight::SetAmbientLight(const glm::vec3& ambient)
{
	ambient_light_ = ambient;
}

const glm::vec3& PointLight::GetPosition()
{
	return position_;
}

const glm::vec3& PointLight::GetDiffuseLight()
{
	return diffuse_light_;
}

const glm::vec3& PointLight::GetAmbientLight()
{
	return ambient_light_;
}