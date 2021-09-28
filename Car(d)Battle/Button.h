#pragma once

#include "stdafx.h"
#include <algorithm>
#include "Text.h"
#include "Shader.h"
#include "utilities/Texture.h"

class Button
{
	bool pressed;
	bool highlight;
	GLuint vao;
	GLuint ebo;
	GLuint vbo;
	CTexture * texture;
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::mat4 m, p, local, transform;
	Text text;
	Shader shader;

	void racalculate_transform();

public:
	Button();
	~Button();
	void draw();
	bool is_hovered(const glm::ivec2&) const;
	void set_projection(const glm::mat4&);
	void set_highlight(bool);
	void set_press(bool);
	void set_pos(const glm::ivec2&);
	void set_size(const glm::ivec2&);
	void set_texture(const std::string&);
	void set_text(const std::string&);
	void set_font(const std::string&);
};
