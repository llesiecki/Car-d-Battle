#pragma once
#include "stdafx.h"

class Shader
{
	std::vector<GLuint> shader_ids;
	GLuint program_id;
	const GLsizei shaders_max;
public:
	Shader();
	~Shader();
	void load(const std::string &, GLenum);
	void link();
	void set(const std::string&, GLboolean);
	void set(const std::string&, GLint);
	void set(const std::string&, GLfloat);
	void enable();
	void disable();
};

