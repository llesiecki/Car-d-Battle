#pragma once
#include "stdafx.h"
#include "Game.h"

class UI
{
	int battle_id;
	std::string user_token;
	std::string nickname;
	std::vector<std::string> players;
	Client network_client;
	Game& game;

	std::map<std::string, std::string> get_server_response(const std::string &, const std::map<std::string, std::string> &);
	int create_battle();
	int join_battle(int, const std::string &);
	int start_battle();
	std::string register_user(const std::string &, const std::string &);
	std::string login_user(const std::string &, const std::string &);

public:
	UI(Game &);
	~UI();

};
