#pragma once

#include "stdafx.h"
#include "Scene.h"
#include <map>
#include <thread>
#include <mutex>
#include "Text3D.h"
#include "Client.h"

enum class Game_state
{
	no_action,
	cards_distribution,
	cards_to_players,
	choose_category,
	show_players_cards,
	compare_by_choosen_category,
	tie_break,
	next_round,
	transfer_cards_to_winner,
	finish
};

enum class Card_translation
{
	no_translation,
	with_flip,
	without_flip
};

class Game
{
	static Game instance;
	Game_state state;
	Cards cards;
	std::vector<std::pair<float, float>> random_translation_vec;//for non regular cards stack
	Scene scene;
	int players_num;
	std::vector<Card> central_stack;
	std::map<int, std::vector<Card>> player_stack;
	std::map<int, std::vector<Card>> player_card;
	std::mutex lock;//threadsafe animations
	int current_player;
	int choosen_category;
	bool* winner;
	bool* loser;
	std::vector<Text3D> texts;
	POINT cursor_pos;
	POINT screen_size;
	Client* network_client;

	void draw_cards_stack(std::vector<Card>& cards_vec);
	void draw_players_cards();
	void draw_players_stacks();
	void distribute_cards();
	void cards_to_players();
	void choose_category();
	void show_players_cards();
	void compare_by_choosen_category();
	void tiebreak();
	void cards_to_winner();

	void move_cards(const Card_translation[]);
	void flip_cards(const bool *);
	friend void OnTimerCallback(int id);
	Game();
public:
	Game(Game&) = delete;
	~Game();
	void set_cursor_pos(int, int);
	void set_screen_size(int, int);
	void load();
	void draw();
	void start(int players_num);
	static Game& get_instance();
};