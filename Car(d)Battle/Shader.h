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
	void enable();
	void disable();
};

