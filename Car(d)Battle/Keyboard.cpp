#include "Keyboard.h"

Keyboard::Keyboard()
	:key_states(), char_lut(), char_lut_shift()
{
	// install kb_hook member function into the Windows hook chain
	hook_kb = SetWindowsHookExW(
		WH_KEYBOARD_LL,
		static_cast<HOOKPROC>(Keyboard::kb_hook),
		GetModuleHandleW(LGAME_NAME),
		0
	);
	hook_mouse = SetWindowsHookExW(
		WH_MOUSE_LL,
		static_cast<HOOKPROC>(Keyboard::kb_hook),
		GetModuleHandleW(LGAME_NAME),
		0
	);

	fill_char_lut();
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
	const BYTE vk_code =
		static_cast<BYTE>(reinterpret_cast<KBDLLHOOKSTRUCT*>(lparam)->vkCode);

	switch (wparam)
	{
	case WM_KEYDOWN:
		Singleton<Keyboard>().notify(vk_code, Key_action::on_press);
		break;
	case WM_KEYUP:
		Singleton<Keyboard>().notify(vk_code, Key_action::on_release);
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

void Keyboard::fill_char_lut()
{
	std::string alpha = "QWERTYUIOPASDFGHJKLZXCVBNM";

	for (const char c : alpha)
	{
		char_lut[c] = c + ('a' - 'A');
		char_lut_shift[c] = c;
	}

	std::string digits = "0123456789";

	for (const char c : digits)
		char_lut[c] = c;

	char_lut_shift['0'] = ')';
	char_lut_shift['1'] = '!';
	char_lut_shift['2'] = '@';
	char_lut_shift['3'] = '#';
	char_lut_shift['4'] = '$';
	char_lut_shift['5'] = '%';
	char_lut_shift['6'] = '^';
	char_lut_shift['7'] = '&';
	char_lut_shift['8'] = '*';
	char_lut_shift['9'] = '(';

	char_lut[VK_SPACE] = ' ';
	char_lut_shift[VK_SPACE] = ' ';

	char_lut[VK_OEM_1] = ';';
	char_lut_shift[VK_OEM_1] = ':';

	char_lut[VK_OEM_2] = '/';
	char_lut_shift[VK_OEM_2] = '?';

	char_lut[VK_OEM_3] = '`';
	char_lut_shift[VK_OEM_3] = '~';

	char_lut[VK_OEM_4] = '[';
	char_lut_shift[VK_OEM_4] = '{';

	char_lut[VK_OEM_5] = '\\';
	char_lut_shift[VK_OEM_5] = '|';

	char_lut[VK_OEM_6] = ']';
	char_lut_shift[VK_OEM_6] = '}';

	char_lut[VK_OEM_7] = '\'';
	char_lut_shift[VK_OEM_7] = '\"';

	char_lut[VK_NUMPAD0] = '0';
	char_lut[VK_NUMPAD1] = '1';
	char_lut[VK_NUMPAD2] = '2';
	char_lut[VK_NUMPAD3] = '3';
	char_lut[VK_NUMPAD4] = '4';
	char_lut[VK_NUMPAD5] = '5';
	char_lut[VK_NUMPAD6] = '6';
	char_lut[VK_NUMPAD7] = '7';
	char_lut[VK_NUMPAD8] = '8';
	char_lut[VK_NUMPAD9] = '9';
	char_lut[VK_MULTIPLY] = '*';
	char_lut[VK_ADD] = '+';
	char_lut[VK_SUBTRACT] = '-';
	char_lut[VK_DIVIDE] = '/';
	char_lut[VK_SEPARATOR] = ',';

	char_lut[VK_OEM_PLUS] = '=';
	char_lut_shift[VK_OEM_PLUS] = '+';

	char_lut[VK_OEM_PERIOD] = '.';
	char_lut_shift[VK_OEM_PERIOD] = '>';

	char_lut[VK_OEM_MINUS] = '-';
	char_lut_shift[VK_OEM_MINUS] = '_';

	char_lut[VK_OEM_COMMA] = ',';
	char_lut_shift[VK_OEM_COMMA] = '<';
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

bool Keyboard::get_key_state(BYTE key)
{
	return key_states[key];
}

char Keyboard::key_to_char(BYTE key, bool shift)
{
	if(shift)
		return char_lut_shift[key];
	return char_lut[key];
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
	if(act == Key_action::on_press)
		key_states[key] = true;
	else if (act == Key_action::on_release)
		key_states[key] = false;

	auto notifier =
		[this](BYTE key, Key_action act) -> void
		{
			Singleton<GL_Context>().obtain();
			handlers_lock.lock();
			for (auto& h : handlers)
			{
				// if handler observes this key
				if (h->key == key)
					h->function(key, act);
			}
			handlers_lock.unlock();
			Singleton<GL_Context>().release();
		};

	std::thread(notifier, key, act).detach();
}
