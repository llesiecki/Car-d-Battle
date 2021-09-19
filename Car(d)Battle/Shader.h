#pragma once
#include "stdafx.h"

class Shader
{
	std::vector<GLuint> shader_ids;
	GLuint program_id;
	const GLsizei shaders_max;

	std::string get_compilation_output(
		GLuint,
		GLsizei,
		std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)>
	);
public:
	Shader();
	~Shader();
	void load(const std::string &, GLenum);
	void link();
	void set(const std::string&, GLboolean) const;
	void set(const std::string&, GLint) const;
	void set(const std::string&, GLfloat) const;
	void set(const std::string&, const glm::mat4&) const;
	void set(const std::string&, const glm::vec3&) const;
	void enable() const;
	void disable() const;
};
