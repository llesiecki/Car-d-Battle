#pragma once

#include "stdafx.h"
#include <mutex>
#include "Blur.h"
#include "Button.h"
#include "Dimmer.h"
#include "Game.h"
#include "Text.h"
#include "TextInput.h"
#include "UI_Interface.h"
#include "Keyboard.h"
#include "Singleton.h"
#include "GL_Context.h"

class MainMenu
{
	enum class State
	{
		login,
		choose_mode,
		singleplayer,
		multiplayer
	};

	State state;
	Game* game;
	UI_Interface* ui;
	glm::ivec2 screen_size;
	const glm::vec2 ref_size;
	const glm::vec2 ref_resolution;
	glm::mat4 mvp;
	std::map<std::string, std::unique_ptr<Button>> buttons;
	std::map<std::string, std::unique_ptr<Text>> texts;
	std::map<std::string, std::unique_ptr<TextInput>> inputs;
	Blur blur;
	Dimmer dimmer;
	std::mutex state_mutex;

	void change_state(State);
	void configure(std::unique_ptr<Button>&);
	void configure(std::unique_ptr<Text>&);
	void configure(std::unique_ptr<TextInput>&);

public:
	MainMenu(const glm::mat4& = glm::mat4(1.0f));
	~MainMenu();
	void draw();
	void set_ui(UI_Interface*);
	void set_mvp(const glm::mat4&);
	void set_screen_size(glm::ivec2);
	void set_cursor_pos(std::pair<float, float>);
	void button_callback(const std::string&);
	void keyboard_callback(BYTE, Keyboard::Key_action);
};
