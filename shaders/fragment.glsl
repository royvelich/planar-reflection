#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
};

in vec3 frag_pos;
in vec3 frag_normal;

uniform PointLight point_light;
uniform Material material;

out vec4 frag_color;

void main()
{
	// Calculate ambient color
	vec3 ambient = point_light.ambient * material.ambient * material.ambient;

	// Calculate diffuse color
	vec3 normal = normalize(frag_normal);
	vec3 light_dir = normalize(point_light.position - frag_pos);
	float factor = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = factor * point_light.diffuse * material.diffuse;

	// Calculate final fragment color
	frag_color = vec4(ambient + diffuse, 1.0f);
};