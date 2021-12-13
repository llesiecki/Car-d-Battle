#include "UI.h"

UI::UI()
	:network_client(80, 512),
	ortho(1.0f),
	battle_id(0),
	kb(Singleton<Keyboard>()),
	screen_size(),
	mainmenu()
{
	game = nullptr;
	kill_threads = false;
	pause = false;
	mainmenu.set_ui(this);
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

	button_start.set_pos({ 0, 0 });
	button_start.set_size({ 400, 40 });
	button_start.set_text("Start Singleplayer");
	button_start.set_texture("textures\\button.bmp");
	button_start.set_font("arial.ttf");
	button_start.set_id("start_sp");
	std::function<void(const std::string&)> fp =
		std::bind(&UI::button_callback, this, std::_Ph<1>());
	button_start.set_press_function(fp);

	button_stop.set_pos({ 60, 60 });
	button_stop.set_size({ 400, 40 });
	button_stop.set_text("Exit");
	button_stop.set_texture("textures\\button.bmp");
	button_stop.set_font("arial.ttf");
	button_stop.set_id("stop");
	fp = std::bind(&UI::button_callback, this, std::_Ph<1>());
	button_stop.set_press_function(fp);

	input.set_font("arial.ttf");
	input.set_id("input");
	input.set_text("Initial text");
	input.set_size({ 300, 30 });
	input.set_pos({ 120.0f, 120.0f });

	text.set_text("Singleplayer");
	text.set_font("arial.ttf");

	text.set_color(glm::vec4(0, 0, 0, 1));
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
	if (key == VK_ESCAPE && action == Keyboard::Key_action::on_press)
	{
		pause ^= true;
		if (game != nullptr)
			game->set_pause(pause);
	}

	if (!pause)
	{
		if (game != nullptr)
			game->key_handler(key, action);
	}

	input.key_handler(key, action);
	button_start.keyboard_callback(key, action);
	button_stop.keyboard_callback(key, action);
	mainmenu.keyboard_callback(key, action);
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
	text.set_mvp(ortho);
	mainmenu.set_screen_size({ x, y });
	if (game != nullptr)
		game->set_screen_size(x, y);
	button_start.set_screen_size({ x, y });
	button_stop.set_screen_size({ x, y });
	input.set_screen_size({ x, y });
	Singleton<GL_Context>().obtain();
	blur.set_size({ x, y });
	Singleton<GL_Context>().release();
}

void UI::set_cursor_pos(float x, float y)
{
	y = screen_size.y - y;
	cursor_pos = { x, y };
	button_stop.set_cursor_pos(cursor_pos);
	button_start.set_cursor_pos(cursor_pos);
	input.set_cursor_pos(cursor_pos);
	mainmenu.set_cursor_pos(cursor_pos);
	if (game != nullptr)
		game->set_cursor_pos(x, y);
}

void UI::render()
{
	auto begin = std::chrono::high_resolution_clock::now();
	Singleton<GL_Context>().obtain();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (game != nullptr)
		game->draw();

	if (pause)
		render_pause_menu();

	glFlush();

	Singleton<GL_Context>().release();
	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

void UI::start()
{
	pause = true;
}

void UI::request_category()
{
	std::thread(&UI::get_current_category, this).detach();
}

void UI::button_callback(const std::string& button_id)
{
	std::cout << "Button " << button_id << " clicked.\n";

	if (button_id == "start_sp" && game == nullptr)
	{
		start_game(4);
	}

	if (button_id == "stop" && game != nullptr)
	{
		Singleton<GL_Context>().obtain();
		delete game;
		game = nullptr;
		Singleton<GL_Context>().release();
	}
}

void UI::exit_game()
{
	Singleton<GL_Context>().obtain();
	GLFWwindow* window = Singleton<GL_Context>().get_window();
	glfwSetWindowShouldClose(window, 1);
	Singleton<GL_Context>().release();
}

void UI::render_pause_menu()
{
	const glm::vec2 ref_size(640, 480);
	const glm::vec2 ref_resolution(1280, 720);

	// case 1 - increase the menu size on larger screens:
	glm::vec2 scale(glm::vec2(screen_size.x, screen_size.y) / ref_resolution);
	scale = glm::vec2(std::min(scale.x, scale.y), std::min(scale.x, scale.y));
	glm::vec2 size = ref_size * scale;

	// case 2 - don't make the menu size too small on smaller screens:
	if (size.x < ref_size.x || size.y < ref_size.y)
	{
		size = ref_size;
	}

	// case 3 - match the menu size with a really small resolution:
	if (size.x > screen_size.x)
	{
		size.x = static_cast<float>(screen_size.x);
		size.y = size.x * ref_size.y / ref_size.x;
	}

	if (size.y > screen_size.y)
	{
		size.y = static_cast<float>(screen_size.y);
		size.x = size.y * ref_size.x / ref_size.y;
	}

	// recalculate scale:
	scale = size / ref_size;
	const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1));

	// calculate origin:
	const glm::vec2 origin(
		(screen_size.x - size.x) / 2,
		(screen_size.y - size.y) / 2
	);
	const glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), glm::vec3(origin, 0));

	// create projection matrix:
	const glm::mat4 menu_ortho = glm::ortho(
		0.0f, static_cast<float>(screen_size.x),
		0.0f, static_cast<float>(screen_size.y)
	) * translate_mat * scale_mat;

	button_start.set_projection(menu_ortho);

	button_stop.set_projection(menu_ortho);

	dimmer.set_mvp(menu_ortho);
	dimmer.set_size(ref_size);

	input.set_projection(menu_ortho);

	glDisable(GL_DEPTH_TEST);
	blur.draw();
	dimmer.draw();
	text.draw();
	button_start.draw();
	button_stop.draw();
	input.draw();
	mainmenu.draw();
	glEnable(GL_DEPTH_TEST);
}

void UI::start_game(int players)
{
	Singleton<GL_Context>().obtain();
	game = new Game();
	game->set_UI(this);
	game->load();
	game->start(players);
	game->set_pause(pause);
	game->set_screen_size(screen_size.x, screen_size.y);
	Singleton<GL_Context>().release();
}
