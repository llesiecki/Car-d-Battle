#include "UI.h"

UI::UI()
	:network_client(80, 512),
	ortho(1.0f),
	battle_id(0),
	kb(Singleton<Keyboard>()),
	screen_size(),
	mainmenu(),
	state(State::mainmenu)
{
	game = nullptr;
	kill_threads = false;
	pause = false;
	mainmenu.set_ui(this);
	pausemenu.set_ui(this);
	network_client.start();
	std::string keys;
	keys = "QWERTYUIOPASDFGHJKLZXCVBNM0123456789";
	std::vector<BYTE> keys_to_observe = {
		VK_LBUTTON, VK_BACK, VK_RETURN, VK_SHIFT, VK_ESCAPE, VK_END,
		VK_DELETE, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_HOME, VK_SPACE,
		VK_RSHIFT, VK_LSHIFT, VK_OEM_1, VK_OEM_2, VK_OEM_3, VK_OEM_4,
		VK_OEM_5, VK_OEM_6, VK_OEM_7, VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,
		VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7,
		VK_NUMPAD8, VK_NUMPAD9, VK_MULTIPLY, VK_ADD, VK_OEM_PLUS,
		VK_SEPARATOR, VK_OEM_PERIOD, VK_SUBTRACT, VK_OEM_MINUS, VK_DIVIDE,
		VK_OEM_COMMA,
	};

	keys_to_observe.insert(keys_to_observe.end(), keys.begin(), keys.end());

	for (const BYTE vk_code : keys_to_observe)
	{
		std::function<void(BYTE, Keyboard::Key_action)> fp =
			std::bind(&UI::key_handler, this, std::_Ph<1>(), std::_Ph<2>());
		int handler_id = kb.observe_key(vk_code, fp);
		handlers.push_back(handler_id);
	}
}

UI::~UI()
{
	kill_threads = true;
	if (game != nullptr)
		delete game;
}

void UI::key_handler(BYTE key, Keyboard::Key_action action)
{
	switch (state)
	{
	case UI::State::mainmenu:

		mainmenu.keyboard_callback(key, action);

		break;
	case UI::State::battle:

		if (key == VK_ESCAPE && action == Keyboard::Key_action::on_press)
		{
			this->set_pause(!pause);

			if (game != nullptr)
				game->set_pause(pause);
		}

		if (pause)
		{
			pausemenu.keyboard_callback(key, action);
		}
		else
		{
			if (game != nullptr)
				game->key_handler(key, action);
		}

		break;
	default:
		break;
	}
}

std::map<std::string, std::string> UI::get_server_response(
	const std::string& script,
	const std::map<std::string, std::string>& query = {}
)
{
	std::map<std::string, std::string> data;
	std::string request = "/" + script + ".php?";
	for (auto element : query)
		request.append(element.first + "=" + element.second + "&");
	// remove last, unnecessary "&" from the request or "?"
	// when no variables provided
	request.pop_back();

	using namespace std::literals::chrono_literals;
	std::chrono::time_point<std::chrono::system_clock> timeout =
		std::chrono::system_clock::now() + 10s;
	while (!network_client.http_get(std::string(SERVER_ADDRESS), request))
	{
		if (std::chrono::system_clock::now() > timeout)
		{
			throw std::runtime_error("Connection with the serwer was lost.\n");
			break;
		}

		if (thread_sleep(kill_threads, 500ms))
			throw std::string("Thread killed");
	}

	std::string& response = network_client.get_response();
	std::regex response_capture("([A-Za-z0-9_]+):([A-Za-z0-9_]*)");
	std::sregex_iterator it(
		response.begin(),
		response.end(),
		response_capture
	);
	std::sregex_iterator end;

	// TODO: Try to use a for each lopp here
	while (it != end)
	{
		if (it->size() != 3)
			continue;
		// map key = 1st capture group match
		// map val = 2nd capture group match
		data[(*it)[1]] = (*it)[2];
		++it;
	}
	if (data["error"].empty())
		return data;
	else
		throw std::runtime_error(data["error"]);
}

int UI::create_battle()
{
	std::map<std::string, std::string> query;
	query["user_token"] = user_token;
	auto response = get_server_response("create_battle", query);
	return std::stoi(response["battle_id"]);
}

int UI::join_battle(int id, const std::string& passwd = "")
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(id);
	query["user_token"] = user_token;
	query["passwd"] = passwd;
	auto response = get_server_response("join_battle", query);
	return std::stoi(response["battle_id"]);
}

int UI::start_battle()
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(battle_id);
	query["user_token"] = user_token;
	auto response = get_server_response("start_battle", query);
	return std::stoi(response["battle_id"]);
}

std::string UI::register_user(
	const std::string& username,
	const std::string& passwd
)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("register_user", query);
	user_token = response["user_token"];
	return response["user_token"];
}

std::string UI::login_user(
	const std::string& username,
	const std::string& passwd
)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("login_user", query);
	user_token = response["user_token"];
	return response["user_token"];
}

void UI::get_current_category()
{
	std::map<std::string, std::string> query;
	query["battle_id"] = std::to_string(battle_id);
	query["user_token"] = user_token;

	std::map<std::string, std::string> response;

	try
	{
		response = get_server_response("get_current_category", query);
	}
	catch (std::string s)
	{
		if (s == "Thread killed")
			return;
	}

	if (game != nullptr)
		game->set_category(std::stoi(response["current_category"]));
}

void UI::set_screen_size(int x, int y)
{
	Singleton<GL_Context>().obtain();

	glViewport(0, 0, x, y);
	//glFlush();

	Singleton<GL_Context>().release();

	screen_size = { x, y };
	ortho =
		glm::ortho(
			0.0f, static_cast<float>(screen_size.x),
			0.0f, static_cast<float>(screen_size.y)
		);
	mainmenu.set_screen_size({ x, y });
	pausemenu.set_screen_size({ x, y });
	if (game != nullptr)
		game->set_screen_size(x, y);
}

void UI::set_cursor_pos(float x, float y)
{
	y = screen_size.y - y;
	cursor_pos = { x, y };
	mainmenu.set_cursor_pos(cursor_pos);
	pausemenu.set_cursor_pos(cursor_pos);
	if (game != nullptr)
		game->set_cursor_pos(x, y);
}

void UI::render()
{
	auto begin = std::chrono::high_resolution_clock::now();
	Singleton<GL_Context>().obtain();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (state)
	{
	case UI::State::mainmenu:

		mainmenu.draw();

		break;
	case UI::State::battle:

		if (game != nullptr)
			game->draw();

		if (pause)
			pausemenu.draw();

		break;
	default:
		break;
	}

	glFlush();

	Singleton<GL_Context>().release();
	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

void UI::start()
{
	state = State::mainmenu;
}

void UI::request_category()
{
	std::thread(&UI::get_current_category, this).detach();
}

void UI::button_callback(const std::string& button_id)
{

}

void UI::exit_game()
{
	Singleton<GL_Context>().obtain();
	GLFWwindow* window = Singleton<GL_Context>().get_window();
	glfwSetWindowShouldClose(window, 1);
	Singleton<GL_Context>().release();
	state = State::mainmenu;
}

void UI::set_pause(bool pause)
{
	this->pause = pause;
}

void UI::leave_battle()
{
	if (game != nullptr)
	{
		Singleton<GL_Context>().obtain();
		delete game;
		game = nullptr;
		Singleton<GL_Context>().release();
	}
	set_pause(false);
	state = State::mainmenu;
}

void UI::start_game(int players)
{
	set_pause(false);
	state = State::battle;
	Singleton<GL_Context>().obtain();
	game = new Game();
	game->set_UI(this);
	game->load();
	game->start(players);
	game->set_pause(pause);
	game->set_screen_size(screen_size.x, screen_size.y);
	Singleton<GL_Context>().release();
}
