#pragma once

#include "stdafx.h"

class Text
{
	std::string text;
	GLuint VBO, VAO;
	glm::vec4 color;

public:
	Text();
	Text(const std::string&);
	Text(const std::string&, glm::vec4);
	~Text();

	void draw();
	void set_text(const std::string&);
	void set_color(glm::vec4);
	void set_color(glm::vec3);
	void set_color(glm::vec3, GLfloat);
	void set_opacity(GLfloat);
};
