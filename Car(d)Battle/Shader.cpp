#include "Shader.h"

Shader::Shader()
	: program_id(glCreateProgram())
{
}

Shader::~Shader()
{
	for (GLuint id : shader_ids)
		glDeleteShader(id);
	disable();
	glDeleteProgram(program_id);
}

void Shader::load(const std::string & source, GLenum type)
{
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
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

void Shader::link()
{
	for (GLuint id : shader_ids)
		glAttachShader(program_id, id);

	glLinkProgram(program_id);

	for (GLuint id : shader_ids)
		glDeleteShader(id);
	shader_ids.clear();
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
