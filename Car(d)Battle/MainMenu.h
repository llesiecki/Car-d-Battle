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
	std::vector<Button> buttons;
	std::vector<Text> texts;
	std::vector<TextInput> inputs;
	Blur blur;
	Dimmer dimmer;
	Game* game;
	UI_Interface* ui;

public:
	void draw();
	void set_ui(UI_Interface*);
	void button_callback(const std::string&);
};

