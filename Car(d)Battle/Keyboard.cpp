#include "Keyboard.h"

Keyboard::Keyboard(HWND h)
	:current_key_state(nullptr), previous_key_state(nullptr), time_pressed(nullptr)
{
	ZeroMemory(key, sizeof(key));
	HgameWindow = h;
	current_key_state = new bool[256]{ false };
	previous_key_state = new bool[256]{ false };
	time_pressed = new signed char[256]{};
	//start a new thread for updating the keyboard state
	keep_updateing = true;
	timer_thread = std::async(std::launch::async, &Keyboard::update_timer, this);
}

Keyboard::~Keyboard()
{
	delete[] current_key_state;
	delete[] previous_key_state;

	keep_updateing = false;
	timer_thread.wait_for(std::chrono::milliseconds(100));

	handlers_lock.lock();
	for (auto h : handlers)
		delete h;
	handlers_lock.unlock();
}

unsigned int Keyboard::update()
{
	/// <summary>
	/// current_key_state - it's boolean representing the current key state
	/// previous_key_state - it's boolean representing the key state from last key scan
	/// time_pressed - signed char, it represents number of scans since the key state chenged:
	///		it goes positive when the key is pressed
	///		it goes negative when the key is released
	/// </summary>
	/// <returns>error code</returns>
	
	if (GetForegroundWindow() == HgameWindow)//game on top, scan all keys
	{
		GetKeyState(0);
		if (!GetKeyboardState(key))
			return GetLastError();
	}
	else//game in background, release all keys
	{
		ZeroMemory(key, sizeof(key));
	}

	std::swap(previous_key_state, current_key_state);

	for (int i = 0; i < 256; i++)
	{
		current_key_state[i] = key[i] >> 7;//leaving only the highest bit
		if (current_key_state[i] != previous_key_state[i])//key state change
		{
			time_pressed[i] = 0;

			handlers_lock.lock();
			for (auto& h : handlers)
			{
				//if handler observes this action, and this key
				if (h->key == i)
				{
					if (h->act == Key_action::on_press && current_key_state[i])
						h->function(i);
					else if (h->act == Key_action::on_release && !current_key_state[i])
						h->function(i);
				}
			}
			handlers_lock.unlock();
		}
		else//no kaystate change
		{
			if (time_pressed[i] < 100 && time_pressed[i] > -100)//overflow protection
			{
				if (time_pressed[i] && current_key_state[i])//the key is being pressed
					time_pressed[i]++;
				if (time_pressed[i] && !current_key_state[i])//the key is released
					time_pressed[i]--;
			}

			handlers_lock.lock();
			for (auto& h : handlers)
			{
				//if handler observes this action, and this key
				if (h->key == i && h->act == Key_action::pressed && current_key_state[i])
					h->function(i);
			}
			handlers_lock.unlock();
		}
	}
	return 0;
}

void Keyboard::update_timer()
{
	while (keep_updateing)
	{
		update();
		std::this_thread::sleep_for(30ms);
	}
}

bool Keyboard::getKeyState(char key)
{
	return current_key_state[key];
}

bool Keyboard::justPressed(char key)
{
	return time_pressed[key] > 0 && time_pressed[key] <= 15;//true, when it's pressed, but only since 15 scans or less
}

unsigned int Keyboard::observe_key(BYTE key, std::function<void(BYTE)> func, Key_action act)
{
	handlers_lock.lock();
	handler* h = new handler{func, key, handlers.size(), act};
	handlers.push_back(h);
	handlers_lock.unlock();
	return h->id;
}

void Keyboard::unobserve_key(unsigned int id)
{
	handlers_lock.lock();
	for (auto it = handlers.begin(); it != handlers.end(); ++it)
	{
		if ((*it)->id == id)
		{
			delete (*it);
			handlers.erase(it);
			break;
		}
	}
	handlers_lock.unlock();
}
