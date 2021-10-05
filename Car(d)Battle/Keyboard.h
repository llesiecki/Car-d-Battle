#pragma once
#include "stdafx.h"
#include <mutex>
#include <future>
#include <chrono>

class Keyboard
{
public:
	enum class Key_action
	{
		pressed,
		on_press,
		on_release
	};

private:
	struct handler
	{
		std::function<void(BYTE, Key_action)> function;
		BYTE key;
		unsigned int id;
	};

	BYTE key[KEYS_NUM];
	bool keep_updateing;
	bool current_key_state[KEYS_NUM];
	bool previous_key_state[KEYS_NUM];
	signed char time_pressed[KEYS_NUM];
	HWND HgameWindow;
	std::mutex handlers_lock;
	std::vector<handler*> handlers;
	std::future<void> timer_thread;

	unsigned int update();
	void update_timer();

public:
	Keyboard(HWND);
	~Keyboard();
	bool getKeyState(char);
	bool justPressed(char);
	unsigned int observe_key(BYTE, std::function<void(BYTE, Key_action)>);
	void unobserve_key(unsigned int);
};

