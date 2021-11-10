#pragma once

#include "stdafx.h"
#include "chrono"
#include "utilities/th_sleep.h"
#include "utilities/Bitmap.h"
#include "Keyboard.h"
#include "Text.h"
#include "Dimmer.h"

class TextInput
{
	bool active;
	bool kill_threads;
	float border_width, text_scale;
	std::pair<float, float>* cursor_pos;
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::vec2 scale;
	glm::vec2 scaled_size;
	glm::mat4 proj, translate, transform;
	std::function<void(const std::string&)> enter_function;
	std::string content;
	std::string id;
	std::chrono::time_point<std::chrono::system_clock> last_input;
	std::atomic<bool> draw_caret;
	std::thread caret_timer;
	Dimmer dimm_left, dimm_right, dimm_up, dimm_down, dimm_center;
	Text text;

	void caret_function();
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
	void set_text(const std::string&);
	void set_font(const std::string&);
	void set_id(const std::string&);
	void set_enter_function(std::function<void(const std::string&)>);
	std::string get_text();
	void set_cursor_pointer(std::pair<float, float>*);
	void key_handler(BYTE, Keyboard::Key_action);
};
