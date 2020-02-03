#ifndef PLANAR_REFLECTION_SHADER_PROGRAM
#define PLANAR_REFLECTION_SHADER_PROGRAM

#include <map>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class ShaderProgram
{
public:
	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	ShaderProgram(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
	virtual ~ShaderProgram();

	bool LoadShaders(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
	void Use();

	GLuint GetProgram() const;

	void SetUniform(const GLchar* name, const glm::vec2& vec);
	void SetUniform(const GLchar* name, const glm::vec3& vec);
	void SetUniform(const GLchar* name, const glm::vec4& vec);
	void SetUniform(const GLchar* name, const glm::mat4& mat);
	void SetUniform(const GLchar* name, const GLfloat value);
	void SetUniform(const GLchar* name, const GLint value);

private:
	void UnloadShaders();
	std::string LoadTextFile(const std::string& file_path);
	void  CheckCompileErrors(GLuint shader, ShaderType type);
	GLint GetUniformLocation(const GLchar* name);
	
	GLuint handle_;
	std::map<std::string, GLint> uniform_locations_;
};

#endif