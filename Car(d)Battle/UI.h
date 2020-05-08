#pragma once
#include "stdafx.h"
#include "Game.h"

class UI
{
	int battle_id;
	int user_id;
	std::vector<int> player_id;
	Game* game;
	Client network_client;

	UI();
	~UI();
	int create_battle();
	int join_battle(int);
	int start_battle();
	bool register_user(std::string&, std::string&);
	bool login_user(std::string&, std::string&);
};
