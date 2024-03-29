#pragma once

#include "stdafx.h"
#include "TrueTypeFont.h"
#include "Singleton.h"

class Text
{
	float scale;
	glm::ivec2 pos;
	std::string text;
	std::string font;
	glm::vec4 color;
	glm::mat4 mvp, transform;
	TrueTypeFont& ttf;

	void recalculate_transform();

public:
	Text();
	Text(const Text&);
	Text(const std::string&);
	Text(const std::string&, glm::vec4);
	~Text();

	Text& operator=(const Text& text);

	void draw();
	void draw(const glm::mat4&);
	void set_text(const std::string&);
	void set_color(glm::vec4);
	void set_color(glm::vec3);
	void set_color(glm::vec3, GLfloat);
	void set_opacity(GLfloat);
	void set_font(const std::string&);
	void set_font(const std::string&, const std::string&);
	void set_mvp(const glm::mat4&);
	void set_pos(const glm::ivec2&);
	void set_scale(float);
	int get_width();
	int get_descent();
};
