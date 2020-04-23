#pragma once

#include "stdafx.h"
#include "Scene.h"
#include <map>
#include <thread>
#include <mutex>
#include "Text3D.h"

enum class Game_state
{
	No_action,
	Cards_distribution,
	Choose_category,
	Show_players_cards,
	Compare_by_choosen_category,
	Tie_break,
	Next_round,
	Transfer_cards_to_winner,
	Finish
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
	std::vector<Text3D> texts;
	POINT cursor_pos;
	POINT screen_size;

	void draw_cards_stack(std::vector<Card>& cards_vec);
	void draw_players_cards();
	void draw_players_stacks();
	void distribute_cards();
	void card_to_player();
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
