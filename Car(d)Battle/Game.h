#pragma once

#include "stdafx.h"
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <thread>
#include <mutex>
#include "Scene.h"
#include "UI_Interface.h"
#include "Cards.h"
#include "Text.h"
#include "utilities/th_sleep.h"
#include "Keyboard.h"

enum class Game_state
{
	no_action,
	cards_distribution,
	cards_to_players,
	choose_category,
	show_players_cards,
	compare_by_choosen_category,
	tie_break,
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
	Game_state old_state;
	Game_state new_state;
	Cards cards;
	//for non regular cards stack
	std::vector<std::pair<float, float>> random_translation_vec;
	Scene scene;
	UI_Interface* ui;
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
	std::vector<thread> threads;
	std::vector<Text> texts;
	bool kill_threads;
	GLFWwindow* window;
	HDC dc;
	HGLRC glrc;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 ortho;
	bool LMB_state;

	void draw_cards_stack(std::vector<Card>&, glm::mat4);
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
	void change_state(Game_state);

	Game(const Game&) = delete;
	Game(Game&&) = delete;
public:
	Game();
	~Game();
	void set_cursor_pos(float, float);
	void set_screen_size(int, int);
	void load();
	void draw();
	void start(int);
	void set_pause(bool);
	void set_UI(UI_Interface*);
	void set_category(int);
	void key_handler(BYTE, Keyboard::Key_action);
};
