#pragma once

#include "stdafx.h"
#include <algorithm>
#include "Text.h"
#include "Shader.h"
#include "utilities/Texture.h"
#include "Keyboard.h"

class Button
{
	bool pressed;
	bool highlight;
	GLuint vao;
	GLuint ebo;
	GLuint vbo;
	Texture* texture;
	Keyboard* kb;
	std::pair<float, float>* cursor_pos;
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::mat4 m, p, local, transform;
	std::string id;
	std::function<void(const std::string&)> press_function;
	Text text;
	Shader shader;

	void racalculate_transform();
	void set_press(bool);

public:
	Button();
	~Button();
	void draw();
	bool is_hovered(const glm::ivec2&) const;
	void set_projection(const glm::mat4&);
	void set_highlight(bool);
	void set_pos(const glm::ivec2&);
	void set_size(const glm::ivec2&);
	void set_texture(const std::string&);
	void set_text(const std::string&);
	void set_font(const std::string&);
	void set_id(const std::string&);
	void set_press_function(std::function<void(const std::string&)>);
	void set_keyboard(Keyboard*);
	void set_cursor_pointer(std::pair<float, float>*);
	void keyboard_callback(BYTE, Keyboard::Key_action);
	void update();
};
