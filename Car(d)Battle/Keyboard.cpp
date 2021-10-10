#include "Keyboard.h"

Keyboard::Keyboard()
{
	// install kb_hook member function into the Windows hook chain
	hook_kb = SetWindowsHookExW(
		WH_KEYBOARD_LL,
		(HOOKPROC)Keyboard::kb_hook,
		GetModuleHandleW(LGAME_NAME),
		0
	);
	hook_mouse = SetWindowsHookExW(
		WH_MOUSE_LL,
		(HOOKPROC)Keyboard::kb_hook,
		GetModuleHandleW(LGAME_NAME),
		0
	);
}

Keyboard::~Keyboard()
{
	handlers_lock.lock();
	for (auto h : handlers)
		delete h;
	handlers_lock.unlock();
	UnhookWindowsHookEx(hook_kb);
	UnhookWindowsHookEx(hook_mouse);
}

LRESULT CALLBACK Keyboard::kb_hook(int next_id, WPARAM wparam, LPARAM lparam)
{
	switch (wparam)
	{
	case WM_KEYDOWN:
		Singleton<Keyboard>().notify(
		static_cast<BYTE>(((KBDLLHOOKSTRUCT*)lparam)->vkCode),
		Key_action::on_press);
		break;
	case WM_KEYUP:
		Singleton<Keyboard>().notify(
			static_cast<BYTE>(((KBDLLHOOKSTRUCT*)lparam)->vkCode),
			Key_action::on_release);
		break;
	case WM_LBUTTONDOWN:
		Singleton<Keyboard>().notify(
			static_cast<BYTE>(VK_LBUTTON),
			Key_action::on_press);
		break;
	case WM_LBUTTONUP:
		Singleton<Keyboard>().notify(
			static_cast<BYTE>(VK_LBUTTON),
			Key_action::on_release);
		break;
	}

	return CallNextHookEx(NULL, next_id, wparam, lparam);
}

void Keyboard::set_focus(int focus)
{
	// call key release on game window focus loss
	if (focus == GL_FALSE)
	{
		handlers_lock.lock();
		for (auto& h : handlers)
			h->function(h->key, Key_action::on_release);
		handlers_lock.unlock();
	}
}

unsigned int Keyboard::observe_key(BYTE key, std::function<void(BYTE, Key_action)> func)
{
	handlers_lock.lock();
	handler* h = new handler{func, key};

	// we need a unique id for the handler,
	// so we can use its address for this purpose - it has to be unique
	h->id = reinterpret_cast<unsigned int>(h);

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

void Keyboard::notify(BYTE key, Key_action act)
{
	handlers_lock.lock();
	for (auto& h : handlers)
	{
		// if handler observes this key
		if (h->key == key)
			h->function(key, act);
	}
	handlers_lock.unlock();
}
