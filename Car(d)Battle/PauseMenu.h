#pragma once
#include "stdafx.h"
#include "UI_Interface.h"
#include "Button.h"
#include "Text.h"
#include "Blur.h"
#include "Dimmer.h"
#include <thread>

class PauseMenu
{
	UI_Interface* ui;
	glm::ivec2 screen_size;
	const glm::vec2 ref_size;
	const glm::vec2 ref_resolution;
	glm::mat4 mvp;
	std::map<std::string, std::unique_ptr<Button>> buttons;
	std::map<std::string, std::unique_ptr<Text>> texts;
	Blur blur;
	Dimmer dimmer;

	void configure(std::unique_ptr<Button>&);
	void configure(std::unique_ptr<Text>&);
	void configure(std::unique_ptr<TextInput>&);

public:
	PauseMenu();
	~PauseMenu();
	void draw();
	void set_ui(UI_Interface*);
	void set_screen_size(glm::ivec2);
	void set_cursor_pos(std::pair<float, float>);
	void button_callback(const std::string&);
	void keyboard_callback(BYTE, Keyboard::Key_action);
};
