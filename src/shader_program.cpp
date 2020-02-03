#include "shader_program.h"
#include "utils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path) :
	handle_(0)
{
	// Load vertex and fragment shaders upon construction
	LoadShaders(vertex_shader_file_path, fragment_shader_file_path);
}

ShaderProgram::~ShaderProgram()
{
	// Unload shader program from GPU before destruction
	UnloadShaders();
}

bool ShaderProgram::LoadShaders(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path)
{
	// Unload shader program before loading the next one
	UnloadShaders();

	// Read vertex and fragment shaders GLSL source files
	std::string vertex_shader_code = Utils::TextFileToString(vertex_shader_file_path);
	std::string fragment_shader_code = Utils::TextFileToString(fragment_shader_file_path);

	// Convert std::string to char pointer
	const GLchar* vertex_shader_code_ptr = vertex_shader_code.c_str();
	const GLchar* fragment_shader_code_ptr = fragment_shader_code.c_str();

	// Create two new shader references in GPU
	GLuint vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader source files to GPU
	glShaderSource(vertex_shader_handle, 1, &vertex_shader_code_ptr, NULL);
	glShaderSource(fragment_shader_handle, 1, &fragment_shader_code_ptr, NULL);

	// Compile vertex shader, and check for compilation errors
	glCompileShader(vertex_shader_handle);
	CheckCompileErrors(vertex_shader_handle, VERTEX);

	// Compile fragment shader, and check for compilation errors
	glCompileShader(fragment_shader_handle);
	CheckCompileErrors(fragment_shader_handle, FRAGMENT);

	// Create a shader program on GPU
	handle_ = glCreateProgram();
	if (handle_ == 0)
	{
		std::cerr << "Unable to create shader program!" << std::endl;
		return false;
	}

	// Attach the two compiled shaders to the shader program
	glAttachShader(handle_, vertex_shader_handle);
	glAttachShader(handle_, fragment_shader_handle);

	// Link shaders to program and check for compilation errors
	glLinkProgram(handle_);
	CheckCompileErrors(handle_, PROGRAM);

	// Delete the shader handles, as we don't need them anymore after linking is complete
	glDeleteShader(vertex_shader_handle);
	glDeleteShader(fragment_shader_handle);

	// Clear uniform variables mappings
	uniform_locations_.clear();

	// Success
	return true;
}

void ShaderProgram::UnloadShaders()
{
	// If we have a valid linking shader program, delete it
	if (handle_ > 0)
	{
		glDeleteProgram(handle_);
		handle_ = 0;
	}
}

void ShaderProgram::Use()
{
	// If we have a valid linking shader program, use it
	if (handle_ > 0)
	{
		glUseProgram(handle_);
	}
}

void  ShaderProgram::CheckCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(handle_, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length);

			// The length includes the NULL character
			std::string errorLog(length, ' ');	// Resize and fill with space character
			glGetProgramInfoLog(handle_, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader program failed to link: " << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			// The length includes the NULL character
			std::string errorLog(length, ' ');  // Resize and fill with space character
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader failed to compile: " << errorLog << std::endl;
		}
	}

}

GLuint ShaderProgram::GetProgram() const
{
	return handle_;
}

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec2& vec)
{
	// Set a vec2 uniform
	GLint location = GetUniformLocation(name);
	glUniform2f(location, vec.x, vec.y);
}

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec3& vec)
{
	// Set a vec3 uniform
	GLint location = GetUniformLocation(name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::SetUniform(const GLchar* name, const glm::vec4& vec)
{
	// Set a vec4 uniform
	GLint location = GetUniformLocation(name);
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetUniform(const GLchar* name, const glm::mat4& mat)
{
	GLint location = GetUniformLocation(name);

	/**
	 * Set mat4f uniform
	 */
	
	// location		=	Location of uniform in shader
	// count		=	How many matrices (1 if not an array of mats)
	// transpose	=	False for glm because column major
	// value		=	The matrix to set for the uniform
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::SetUniform(const GLchar* name, const GLfloat value)
{
	// Set float
	GLint location = GetUniformLocation(name);
	glUniform1f(location, value);
}


void ShaderProgram::SetUniform(const GLchar* name, const GLint value)
{
	// Set integer
	GLint location = GetUniformLocation(name);
	glUniform1i(location, value);
}

GLint ShaderProgram::GetUniformLocation(const GLchar* name)
{
	// Check if we have already defined a uniform variable under this name
	std::map<std::string, GLint>::iterator it = uniform_locations_.find(name);

	// If the uniform variable doesn't exist in the current shader program, create one
	if (it == uniform_locations_.end())
	{
		uniform_locations_[name] = glGetUniformLocation(handle_, name);
	}

	// Return the handle of the uniform
	return uniform_locations_[name];
}