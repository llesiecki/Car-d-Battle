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

	std::string output = std::move(
		get_compilation_output(program_id, 1024, glGetShaderInfoLog)
	);

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

	std::string output = std::move(
		get_compilation_output(program_id, 1024, glGetProgramInfoLog)
	);
	
	if (!output.empty())
		throw output;

	GLsizei count;
	GLuint* shaders = new GLuint[shaders_max];

	glGetAttachedShaders(program_id, shaders_max, &count, shaders);

	for (int i = 0; i < count && i < shaders_max; ++i)
		glDetachShader(program_id, shaders[i]);

	delete[] shaders;

	for (GLuint id : shader_ids)
		glDeleteShader(id);
	
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

std::string Shader::get_compilation_output(
	GLuint id,
	GLsizei buffer_size,
	std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> GLgetInfoLog
)
{
	GLint success;
	std::string output;

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		std::vector<GLchar> info_log(buffer_size);
		GLsizei bytes_written;
		GLgetInfoLog(id, buffer_size, &bytes_written, info_log.data());
		if(bytes_written)
			return("ERROR::SHADER::COMPILATION_FAILED:\n" +
				std::string(info_log.begin(), info_log.end()));
	}

	return std::string();
}
