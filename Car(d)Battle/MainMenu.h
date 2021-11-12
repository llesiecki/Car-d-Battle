#pragma once

#include "stdafx.h"
#include "Blur.h"
#include "Button.h"
#include "Dimmer.h"
#include "Game.h"
#include "Text.h"
#include "TextInput.h"
#include "UI_Interface.h"

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
	std::map<std::string, Button> buttons;
	std::map<std::string, Text> texts;
	std::map<std::string, TextInput> inputs;
	Blur blur;
	Dimmer dimmer;

public:
	MainMenu();
	~MainMenu();
	void draw();
	void set_ui(UI_Interface*);
	void set_screen_size(glm::ivec2);
	void button_callback(const std::string&);
};

