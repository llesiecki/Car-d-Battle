#pragma once

#include "stdafx.h"
#include "utilities/Texture.h"
#include "Keyboard.h"
#include "Text.h"
#include "Shader.h"

class TextInput
{
	GLuint vao;
	GLuint ebo;
	GLuint vbo;
	Texture* texture;
	Keyboard* kb;
	std::pair<float, float>* cursor_pos;
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::vec2 scale;
	glm::vec2 scaled_size;
	glm::mat4 proj, translate, transform;
	std::function<void(const std::string&)> enter_function;
	std::string content;
	std::string ids;
	Text text;
	Shader shader;

	void racalculate_transform();

public:
	TextInput();
	~TextInput();
	void draw();
	bool is_hovered(const glm::ivec2&) const;
	void set_projection(const glm::mat4&);
	void set_pos(const glm::ivec2&);
	void set_size(const glm::ivec2&);
	void set_scale(const glm::vec2&);
	void set_texture(const std::string&);
	void set_text(const std::string&);
	void set_font(const std::string&);
	void set_id(const std::string&);
	void set_enter_function(std::function<void(const std::string&)>);
	void set_keyboard(Keyboard*);
	std::string get_text();
	void set_cursor_pointer(std::pair<float, float>*);
	void keyboard_callback(BYTE, Keyboard::Key_action);
};
