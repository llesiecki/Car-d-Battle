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
	
	GLint success;
	std::string output;

	glGetShaderiv(shader_ids.back(), GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint buffer_size;
		glGetShaderiv(shader_ids.back(), GL_INFO_LOG_LENGTH, &buffer_size);

		if (buffer_size != 0)
		{
			// log_len is returned without the null terminator,
			// so it's worth to add 1
			std::vector<GLchar> info_log(buffer_size + 1);

			GLsizei bytes_written;
			glGetShaderInfoLog(shader_ids.back(), buffer_size, &bytes_written, info_log.data());

			if (bytes_written)
				output = "ERROR::SHADER::COMPILATION_FAILED:\n" +
				std::string(info_log.begin(), info_log.end());
		}
	}

	if (!output.empty())
	{
		glDeleteShader(shader_ids.back());
		shader_ids.pop_back();
		throw output;
	}
}

void Shader::link()
{
	for (GLuint id : shader_ids)
		glAttachShader(program_id, id);

	glLinkProgram(program_id);

	GLint buffer_size;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &buffer_size);

	if (buffer_size != 0)
	{
		// log_len is returned without the null terminator,
		// so it's worth to add 1
		std::vector<GLchar> info_log(buffer_size + 1);

		GLsizei bytes_written;
		glGetProgramInfoLog(program_id, buffer_size, &bytes_written,
			info_log.data());

		if (bytes_written != 0)
			throw("ERROR::SHADER::LINK:\n" +
				std::string(info_log.begin(), info_log.end()));
	}

	// At this point all shaders are successfully
	// compiled and linked into the shader program,
	// so the shader objects can be detached and deleted

	for (GLuint id : shader_ids)
	{
		glDetachShader(program_id, id);
		glDeleteShader(id);
	}

	shader_ids.clear();
}

void Shader::set(const std::string & name, GLboolean value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()),
		static_cast<GLint>(value)
	);
}

void Shader::set(const std::string & name, GLint value) const
{
	glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set(const std::string & name, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set(const std::string& name, const glm::mat4 & value) const
{
	glUniformMatrix4fv(
		glGetUniformLocation(program_id, name.c_str()),
		1,
		GL_FALSE,
		glm::value_ptr(value)
	);
}

void Shader::set(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(
		glGetUniformLocation(program_id, name.c_str()),
		1,
		glm::value_ptr(value)
	);
}

void Shader::enable() const
{
	glUseProgram(program_id);
}

void Shader::disable() const
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	if(id == program_id)
		glUseProgram(0);
}
