#pragma once
#include "stdafx.h"
#include <chrono>
#include <mutex>
#include <array>
#include <climits>
#include "Singleton.h"
#include "GL_Context.h"

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

	HHOOK hook_kb;
	HHOOK hook_mouse;
	std::mutex handlers_lock;
	std::vector<handler*> handlers;
	std::array<bool, UCHAR_MAX> key_states;
	std::array<char, UCHAR_MAX> char_lut;
	std::array<char, UCHAR_MAX> char_lut_shift;

	void fill_char_lut();
	Keyboard();
public:
	friend Keyboard& Singleton<Keyboard>();

	~Keyboard();
	unsigned int observe_key(BYTE, std::function<void(BYTE, Key_action)>);
	void unobserve_key(unsigned int);
	void notify(BYTE, Key_action);
	void set_focus(int);
	bool get_key_state(BYTE);
	char key_to_char(BYTE, bool);
	static LRESULT CALLBACK kb_hook(int next_id, WPARAM wparam, LPARAM lparam);
};
