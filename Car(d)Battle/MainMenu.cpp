#include "MainMenu.h"

MainMenu::MainMenu()
	:screen_size(1, 1)
{
	state = State::login;
	game = nullptr;
	ui = nullptr;
}

MainMenu::~MainMenu()
{
	if (game != nullptr)
		delete game;
}

void MainMenu::draw()
{
	switch (state)
	{
	case MainMenu::State::login:
		inputs["login"].draw();
		inputs["password"].draw();
		buttons["password"].draw();
		break;
	case MainMenu::State::choose_mode:
		buttons["singleplayer"].draw();
		buttons["multiplayer"].draw();
		break;
	case MainMenu::State::singleplayer:
		buttons["1_opponent"].draw();
		buttons["2_opponents"].draw();
		buttons["3_opponents"].draw();
		break;
	case MainMenu::State::multiplayer:
		buttons["create_battle"].draw();
		buttons["launch_battle"].draw();
		buttons["join_battle"].draw();
		buttons["leave_battle"].draw();
		break;
	default:
		break;
	}
}

void MainMenu::set_ui(UI_Interface* ui)
{
	this->ui = ui;
}

void MainMenu::set_screen_size(glm::ivec2 size)
{
	screen_size = size;
}

void MainMenu::button_callback(const std::string&)
{
}
