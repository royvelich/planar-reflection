#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 frag_normal;

void main()
{
	// Pass to fragment shader the associated vertex position
	frag_pos = vec3(model * vec4(pos, 1.0f));

	// Pass to fragment shader the associated vertex normal
	frag_normal = mat3(model) * normal;

	// Run position through pipeline
	gl_Position = projection * view * model * vec4(pos, 1.0f);
}