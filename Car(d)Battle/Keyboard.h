#include "stdafx.h"
#include <mutex>
#include <future>

#pragma once

enum class Key_action
{
	pressed,
	on_press,
	on_release
};

class Keyboard
{
	struct handler
	{
		std::function<void(BYTE)> function;
		BYTE key;
		unsigned int id;
		Key_action act;
	};

	BYTE key[256];
	bool keep_updateing;
	bool* current_key_state;
	bool* previous_key_state;
	signed char* time_pressed;
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
	unsigned int observe_key(BYTE, std::function<void(BYTE)>, Key_action);
	void unobserve_key(unsigned int);
};

