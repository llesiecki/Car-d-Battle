#pragma once
#include "stdafx.h"
#include <chrono>
#include <mutex>
#include "Singleton.h"

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

	Keyboard();
public:
	friend Keyboard& Singleton<Keyboard>();

	~Keyboard();
	unsigned int observe_key(BYTE, std::function<void(BYTE, Key_action)>);
	void unobserve_key(unsigned int);
	void notify(BYTE, Key_action);
	void set_focus(int);
	static LRESULT CALLBACK kb_hook(int next_id, WPARAM wparam, LPARAM lparam);
};
