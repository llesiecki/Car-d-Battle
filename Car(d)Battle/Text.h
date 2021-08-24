#pragma once

#include "stdafx.h"
#include "TrueTypeFont.h"
#include "Singleton.h"

class Text
{
	std::string text;
	std::string font;
	GLuint VBO, VAO;
	glm::vec4 color;
	TrueTypeFont& ttf;

public:
	Text();
	Text(const std::string&);
	Text(const std::string&, glm::vec4);
	~Text();

	void draw(const glm::mat4&);
	void set_text(const std::string&);
	void set_color(glm::vec4);
	void set_color(glm::vec3);
	void set_color(glm::vec3, GLfloat);
	void set_opacity(GLfloat);
	void set_font(const std::string&);
	void set_font(const std::string&, const std::string&);
};
