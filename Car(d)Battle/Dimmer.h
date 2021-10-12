#pragma once

#include "stdafx.h"
#include "Shader.h"

class Dimmer
{
	GLuint vbo, vao, ebo;
	GLfloat opacity;
	glm::vec2 size;
	glm::mat4 mvp;
	Shader shader;

public:
	Dimmer();
	~Dimmer();
	void draw();
	void set_mvp(const glm::mat4&);
	void set_opacity(GLfloat);
	void set_size(glm::vec2);
};
