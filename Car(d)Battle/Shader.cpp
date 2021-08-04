#include "Shader.h"

Shader::Shader()
	: program_id(glCreateProgram()), shaders_max(64)
{
}

Shader::~Shader()
{
	for (GLuint id : shader_ids)
		glDeleteShader(id);
	disable();
	glDeleteProgram(program_id);
}

void Shader::load(const std::string & path, GLenum type)
{
	std::ifstream file(path);
	std::string source((std::istreambuf_iterator<char>(file)), // Extra parentheses due to "The Most Vexing Parse Problem"
		std::istreambuf_iterator<char>()); // In short, C++ would interpret this line as a function pointer

	shader_ids.push_back(glCreateShader(type));
	const char* p_source = source.c_str();

	glShaderSource(shader_ids.back(), 1, &p_source, NULL);
	glCompileShader(shader_ids.back());

	int success;
	char info_log[512];
	glGetShaderiv(shader_ids.back(), GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader_ids.back(), sizeof(info_log), NULL, info_log);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
		glDeleteShader(shader_ids.back());
		shader_ids.pop_back();
	}
}

void Shader::link()
{
	for (GLuint id : shader_ids)
		glAttachShader(program_id, id);

	glLinkProgram(program_id);

	GLsizei count;
	std::shared_ptr<GLuint> shaders = std::make_shared<GLuint>(shaders_max);

	glGetAttachedShaders(program_id, shaders_max, &count, shaders.get());

	for (int i = 0; i < count; ++i)
		glDetachShader(program_id, shaders.get()[i]);

	for (GLuint id : shader_ids)
		glDeleteShader(id);
	shader_ids.clear();
}

void Shader::set(const std::string & name, GLboolean value)
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()),
		static_cast<GLint>(value)
	);
}

void Shader::set(const std::string & name, GLint value)
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set(const std::string & name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set(const std::string& name, const glm::mat4 & value)
{
	glUniformMatrix4fv(
		glGetUniformLocation(program_id, name.c_str()),
		1,
		GL_FALSE,
		glm::value_ptr(value)
	);
}

void Shader::set(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(
		glGetUniformLocation(program_id, name.c_str()),
		1,
		glm::value_ptr(value)
	);
}

void Shader::enable()
{
	glUseProgram(program_id);
}

void Shader::disable()
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	if(id == program_id)
		glUseProgram(0);
}
