#include "Shader.h"

Shader::Shader()
	: id(glCreateShader(GL_VERTEX_SHADER))
{
}

Shader::Shader(const std::string & source)
	: Shader()
{
	load(source);
}

void Shader::load(const std::string & source)
{
	const char* p_source = source.c_str();
	glShaderSource(id, 1, &p_source, NULL);
	glCompileShader(id);

	int  success;
	char info_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

void Shader::enable()
{
}

void Shader::disable()
{
}
