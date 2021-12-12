#pragma once
#include <regex>
#include <thread>
#include "stdafx.h"
#include "Client.h"
#include "Keyboard.h"
#include "Game.h"
#include "UI_Interface.h"
#include "Text.h"
#include "Button.h"
#include "Singleton.h"
#include "GL_Context.h"
#include "Dimmer.h"
#include "Blur.h"
#include "TextInput.h"
#include "MainMenu.h"

class UI
	:public UI_Interface
{
	int battle_id;
	std::string user_token;
	std::string nickname;
	std::vector<std::string> players;
	Client network_client;

	Game* game;

	bool pause;
	Keyboard& kb;
	std::vector<unsigned int> handlers;
	bool kill_threads;
	POINT screen_size;
	std::pair<float, float> cursor_pos;
	glm::mat4 ortho;
	Text text;
	Button button_start, button_stop;
	Dimmer dimmer;
	Blur blur;
	TextInput input;
	MainMenu mainmenu;

	void key_handler(BYTE, Keyboard::Key_action);
	std::map<std::string, std::string> get_server_response(
		const std::string &,
		const std::map<std::string, std::string> &
	);
	int create_battle();
	int join_battle(int, const std::string &);
	int start_battle();
	std::string register_user(const std::string &, const std::string &);
	std::string login_user(const std::string &, const std::string &);
	void render_pause_menu();
	void start_game(int);

	UI();
public:
	friend UI& Singleton<UI>();
	~UI();

	void get_current_category();
	void set_screen_size(int, int);
	void set_cursor_pos(float, float);
	void render();
	void start();
	//interface implementation:
	void request_category();
	void button_callback(const std::string&);
	void exit_game();
};
