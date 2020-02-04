#include "material.h"

Material::Material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color) :
	ambient_color_(ambient_color),
	diffuse_color_(diffuse_color)
{
	
}

Material::~Material()
{
	
}

void Material::SetDiffuseColor(const glm::vec3& diffuse_color)
{
	diffuse_color_ = diffuse_color;
}

void Material::SetAmbientColor(const glm::vec3& ambient_color)
{
	ambient_color_ = ambient_color;
}

const glm::vec3& Material::GetAmbientColor() const
{
	return ambient_color_;
}

const glm::vec3& Material::GetDiffuseColor() const
{
	return diffuse_color_;
}
