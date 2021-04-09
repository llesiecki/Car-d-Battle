#pragma once
#include "stdafx.h"
#include "Game.h"

class UI
{
	struct player
	{
		int id;
		std::string nickname;
	};

	int battle_id;
	int user_id;
	std::string nickname;
	std::vector<player> players;
	Game* game;
	Client network_client;

	int create_battle();
	int join_battle(int);
	int start_battle();
	bool register_user(std::string&, std::string&);
	bool login_user(std::string&, std::string&);

public:
	UI();
	~UI();
};
