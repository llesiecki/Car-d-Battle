#include "UI.h"

UI::UI()
	:network_client(80, 512),
	game(game),
	battle_id(0),
	kb(FindWindowW(NULL, LGAME_NAME)),
	screen_size()
{
	game = new Game();
	game->set_UI(this);
	kill_threads = false;
	pause = false;
	network_client.start();
	std::string keys_to_observe = "QWERTYUIOPASDFGHJKLZXCVBNM0123456789";
	keys_to_observe += {
		VK_LBUTTON, VK_BACK, VK_RETURN, VK_SHIFT, VK_ESCAPE, VK_END,
		VK_DELETE, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_HOME
	};

	for (const char vk_code : keys_to_observe)
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
	delete game;
}

void UI::key_handler(BYTE key, Keyboard::Key_action action)
{
	if (key == VK_ESCAPE && action == Keyboard::Key_action::on_press)
	{
		pause ^= true;
		game->set_pause(pause);
	}

	if (!pause)
	{
		game->key_handler(key, action);
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

	std::chrono::time_point<std::chrono::system_clock> timeout =
		std::chrono::system_clock::now() + 10s;
	while (!network_client.http_get(std::string(SERVER_ADDRESS), request))
	{
		if (std::chrono::system_clock::now() > timeout)
		{
			throw std::runtime_error("Connection with the serwer was lost.\n");
			break;
		}

		if(thread_sleep(kill_threads, 500ms))
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

int UI::join_battle(int id, const std::string & passwd = "")
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
	const std::string & username,
	const std::string & passwd
)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("register_user", query);
	return response["user_token"];
}

std::string UI::login_user(
	const std::string & username,
	const std::string & passwd
)
{
	std::map<std::string, std::string> query;
	query["username"] = username;
	query["passwd"] = passwd;
	auto response = get_server_response("login_user", query);
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
	catch(std::string s)
	{
		if (s == "Thread killed")
			return;
	}

	if (game)
		game->set_category(std::stoi(response["current_category"]));
}

void UI::set_screen_size(int x, int y)
{
	glViewport(0, 0, x, y);
	screen_size = { x, y };
	game->set_screen_size(x, y);
}

void UI::set_cursor_pos(float x, float y)
{
	y = screen_size.y - y;
	cursor_pos = { x, y };
	game->set_cursor_pos(x, y);
}

void UI::render()
{
	if (pause)
		render_pause_menu();
	game->draw();
}

void UI::start()
{
	game->load();
	game->start(4);
}

void UI::request_category()
{
	std::thread(&UI::get_current_category, this).detach();
}

void UI::render_pause_menu()
{
	
}
