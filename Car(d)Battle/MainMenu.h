#pragma once

#include "stdafx.h"
#include "Blur.h"
#include "Button.h"
#include "Dimmer.h"
#include "Game.h"
#include "Text.h"
#include "TextInput.h"
#include "UI_Interface.h"
#include "Keyboard.h"

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
	Keyboard* kb;
	std::pair<float, float>* cursor_pos;
	glm::ivec2 screen_size;
	glm::mat4 mvp;
	std::map<std::string, std::unique_ptr<Button>> buttons;
	std::map<std::string, std::unique_ptr<Text>> texts;
	std::map<std::string, std::unique_ptr<TextInput>> inputs;
	Blur blur;
	Dimmer dimmer;

	void change_state(State);
	void configure(std::unique_ptr<Button>&);
	void configure(std::unique_ptr<Text>&);
	void configure(std::unique_ptr<TextInput>&);

public:
	MainMenu(Keyboard*, std::pair<float, float>*, const glm::mat4&);
	~MainMenu();
	void draw();
	void set_ui(UI_Interface*);
	void set_mvp(const glm::mat4&);
	void set_screen_size(const glm::ivec2);
	void button_callback(const std::string&);
};
