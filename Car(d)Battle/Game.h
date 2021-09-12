#pragma once

#include "stdafx.h"
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <thread>
#include <mutex>
#include "Scene.h"
#include "Client.h"
#include "UI.h"
#include "Cards.h"
#include "Singleton.h"
#include "Text.h"

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
	Game_state state;
	Cards cards;
	std::vector<std::pair<float, float>> random_translation_vec;//for non regular cards stack
	Scene scene;
	UI ui;
	int players_num;
	std::vector<Card> central_stack;
	std::map<int, std::vector<Card>> player_stack;
	std::map<int, std::vector<Card>> player_card;
	std::mutex animations_lock;//threadsafe animations
	std::mutex gl_lock;//using just one GL context makes work much simpler
	int current_player;
	int choosen_category;
	bool* winner;
	bool* loser;
	bool pause;
	std::pair<float, float> cursor_pos;
	POINT screen_size;
	Client* network_client;
	std::vector<thread> threads;
	std::vector<Text> texts;
	bool kill_threads;
	GLFWwindow* window;
	HDC dc;
	HGLRC glrc;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 ortho;

	bool thread_sleep_ms(unsigned int);
	void draw_cards_stack(std::vector<Card>& cards_vec, glm::mat4);
	void draw_players_cards();
	void draw_players_stacks();
	void distribute_cards();
	void cards_to_players();
	void choose_category();
	void show_players_cards();
	void compare_by_choosen_category();
	void tiebreak();
	void cards_to_winner();
	void clean();
	void move_cards(const Card_translation[]);
	void flip_cards(const bool[]);

	Game();
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	~Game();
public:
	friend Game& Singleton<Game>();
	void set_cursor_pos(double, double);
	void set_screen_size(int, int);
	void load();
	void draw();
	void start(int players_num);
	void set_pause(bool);
};
