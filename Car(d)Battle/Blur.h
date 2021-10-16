#pragma once

#include "stdafx.h"
#include "Shader.h"

class Blur
{
	GLuint vao, vbo, ebo;
	GLuint frame_tex;
	glm::ivec2 size;
	Shader shader;

public:
	Blur();
	~Blur();
	void set_size(glm::ivec2);
	void draw();
};
