#include "MainMenu.h"

MainMenu::MainMenu(const glm::mat4& mvp)
	:screen_size(1, 1), mvp(mvp)
{
	state = State::login;
	game = nullptr;
	ui = nullptr;

	dimmer.set_mvp(mvp);
	change_state(State::login);
}

MainMenu::~MainMenu()
{
	if (game != nullptr)
		delete game;
}

void MainMenu::change_state(State new_state)
{
	state_mutex.lock();

	switch (state)
	{
	case MainMenu::State::login:
		assert(
			new_state == State::login ||
			new_state == State::choose_mode
		);
		break;

	case MainMenu::State::choose_mode:
		assert(
			new_state == State::login ||
			new_state == State::singleplayer ||
			new_state == State::multiplayer
		);
		break;

	case MainMenu::State::singleplayer:
		assert(new_state == State::choose_mode);
		break;

	case MainMenu::State::multiplayer:
		assert(new_state == State::choose_mode);
		break;

	default:
		break;
	}

	state = new_state;

	buttons.clear();
	inputs.clear();
	texts.clear();

	GLuint bottom_offset = 150;
	GLuint left_offset = 315;

	if (new_state == State::login)
	{

		std::unique_ptr<TextInput> input_ptr = std::make_unique<TextInput>();
		inputs["login"] = std::move(input_ptr);
		configure(inputs["login"]);
		inputs["login"]->set_pos({ left_offset, bottom_offset + 65 });
		inputs["login"]->set_text("Nickname");

		input_ptr = std::make_unique<TextInput>();
		inputs["password"] = std::move(input_ptr);
		configure(inputs["password"]);
		inputs["password"]->set_pos({ left_offset + 225, bottom_offset + 65 });
		inputs["password"]->set_text("Password");
		inputs["password"]->set_id("input_login");

		std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
		buttons["login"] = std::move(button_ptr);
		configure(buttons["login"]);
		buttons["login"]->set_pos({ left_offset + 450, bottom_offset + 65 });
		buttons["login"]->set_text("Login");
		buttons["login"]->set_id("button_login");
	}

	if (new_state == State::choose_mode)
	{
		std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
		buttons["singleplayer"] = std::move(button_ptr);
		configure(buttons["singleplayer"]);
		buttons["singleplayer"]->set_pos({ left_offset, bottom_offset + 65 });
		buttons["singleplayer"]->set_text("Singleplayer");
		buttons["singleplayer"]->set_id("singleplayer");

		button_ptr = std::make_unique<Button>();
		buttons["multiplayer"] = std::move(button_ptr);
		configure(buttons["multiplayer"]);
		buttons["multiplayer"]->set_pos({ left_offset + 225, bottom_offset + 65 });
		buttons["multiplayer"]->set_text("Multiplayer");
		buttons["multiplayer"]->set_id("multiplayer");
	}

	if (new_state == State::singleplayer)
	{
		std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
		buttons["1_opponent"] = std::move(button_ptr);
		configure(buttons["1_opponent"]);
		buttons["1_opponent"]->set_pos({ left_offset + 400, bottom_offset + 195 });
		buttons["1_opponent"]->set_size({ 40, 40 });
		buttons["1_opponent"]->set_text("1");

		button_ptr = std::make_unique<Button>();
		buttons["2_opponents"] = std::move(button_ptr);
		configure(buttons["2_opponents"]);
		buttons["2_opponents"]->set_pos({ left_offset + 400, bottom_offset + 130 });
		buttons["2_opponents"]->set_size({ 40, 40 });
		buttons["2_opponents"]->set_text("2");

		button_ptr = std::make_unique<Button>();
		buttons["3_opponents"] = std::move(button_ptr);
		configure(buttons["3_opponents"]);
		buttons["3_opponents"]->set_pos({ left_offset + 400, bottom_offset + 65 });
		buttons["3_opponents"]->set_size({ 40, 40 });
		buttons["3_opponents"]->set_text("3");

		std::unique_ptr<Text> text_ptr = std::make_unique<Text>();
		texts["opponents_num"] = std::move(text_ptr);
		configure(texts["opponents_num"]);
		texts["opponents_num"]->set_text("Choose nuber of opponents:");
		// TODO: Extend the Text class with a "set_pos" member function
		// texts["opponents_num"]->set_pos({ 150, 30 });
	}

	if (new_state == State::multiplayer)
	{
		std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
		buttons["create_battle"] = std::move(button_ptr);
		configure(buttons["create_battle"]);
		buttons["create_battle"]->set_pos({ left_offset + 450, bottom_offset + 130 });
		buttons["create_battle"]->set_text("Create Battle");

		button_ptr = std::make_unique<Button>();
		buttons["start_battle"] = std::move(button_ptr);
		configure(buttons["start_battle"]);
		buttons["start_battle"]->set_pos({ left_offset + 450, bottom_offset + 65 });
		buttons["start_battle"]->set_text("Start Battle");

		button_ptr = std::make_unique<Button>();
		buttons["join_battle"] = std::move(button_ptr);
		configure(buttons["join_battle"]);
		buttons["join_battle"]->set_pos({ left_offset , bottom_offset + 65 });
		buttons["join_battle"]->set_text("Join Battle");

		button_ptr = std::make_unique<Button>();
		buttons["leave_battle"] = std::move(button_ptr);
		configure(buttons["leave_battle"]);
		buttons["leave_battle"]->set_pos({ left_offset + 225, bottom_offset });
		buttons["leave_battle"]->set_text("Leave Battle");

		std::unique_ptr<TextInput> input_ptr = std::make_unique<TextInput>();
		inputs["battle_id"] = std::move(input_ptr);
		configure(inputs["battle_id"]);
		inputs["battle_id"]->set_pos({ left_offset , bottom_offset + 130 });
		inputs["battle_id"]->set_text("Battle ID");
	}

	state_mutex.unlock();
}

void MainMenu::configure(std::unique_ptr<Button>& button)
{
	button->set_size({ 200, 40 });
	button->set_texture("textures\\button.bmp");
	button->set_font("arial.ttf");
	button->set_projection(mvp);
	button->set_screen_size(screen_size);
	std::function<void(const std::string&)> fp =
		std::bind(&MainMenu::button_callback, this, std::_Ph<1>());
	button->set_press_function(fp);
}

void MainMenu::configure(std::unique_ptr<Text>& text)
{
	text->set_font("arial.ttf");
	text->set_color(glm::vec4(0, 0, 0, 1));
	text->set_mvp(mvp);
}

void MainMenu::configure(std::unique_ptr<TextInput>& input)
{
	input->set_font("arial.ttf");
	input->set_size({ 200, 40 });
	input->set_projection(mvp);
	input->set_screen_size(screen_size);
	std::function<void(const std::string&)> fp =
		std::bind(&MainMenu::button_callback, this, std::_Ph<1>());
	input->set_enter_function(fp);
}

void MainMenu::draw()
{
	if (game != nullptr)
		game->draw();
	blur.draw();
	dimmer.draw();

	state_mutex.lock();

	switch (state)
	{
	case State::login:
		inputs["login"]->draw();
		inputs["password"]->draw();
		buttons["login"]->draw();
		break;
	case State::choose_mode:
		buttons["singleplayer"]->draw();
		buttons["multiplayer"]->draw();
		break;
	case State::singleplayer:
		buttons["1_opponent"]->draw();
		buttons["2_opponents"]->draw();
		buttons["3_opponents"]->draw();
		break;
	case State::multiplayer:
		buttons["create_battle"]->draw();
		buttons["start_battle"]->draw();
		buttons["join_battle"]->draw();
		buttons["leave_battle"]->draw();
		inputs["battle_id"]->draw();
		break;
	default:
		break;
	}

	state_mutex.unlock();
}

void MainMenu::set_ui(UI_Interface* ui)
{
	this->ui = ui;
}

void MainMenu::set_mvp(const glm::mat4& mvp)
{
	this->mvp = mvp;

	dimmer.set_mvp(mvp);

	for (auto& kv : inputs)
		kv.second->set_projection(mvp);

	for (auto& kv : buttons)
		kv.second->set_projection(mvp);
}

void MainMenu::set_screen_size(const glm::ivec2 size)
{
	screen_size = size;
	blur.set_size(size);
	dimmer.set_size(size);
	if (game != nullptr)
		game->set_screen_size(size.x, size.y);

	for (auto& kv : inputs)
		kv.second->set_screen_size(size);

	for (auto& kv : buttons)
		kv.second->set_screen_size(size);
}

void MainMenu::set_cursor_pos(std::pair<float, float> cursor_pos)
{
	for (auto& kv : buttons)
		kv.second->set_cursor_pos(cursor_pos);

	for (auto& kv : inputs)
		kv.second->set_cursor_pos(cursor_pos);
}

void MainMenu::button_callback(const std::string& id)
{
	Singleton<GL_Context>().obtain();

	if (id == "singleplayer")
	{
		change_state(State::singleplayer);
	}
	else if (id == "multiplayer")
	{
		change_state(State::multiplayer);
	}
	else if (id == "button_login" || id == "input_login")
	{
		std::string login = inputs["login"]->get_text();
		std::string passwd = inputs["password"]->get_text();
		std::string token = ui->login_user(login, passwd);
		if (!token.empty())
			change_state(State::choose_mode);
	}
	else if (id == "1_opponent")
	{
		ui->start_game(1);
	}
	else if (id == "2_opponents")
	{
		ui->start_game(2);
	}
	else if (id == "3_opponents")
	{
		ui->start_game(3);
	}
	else if (id == "create_battle")
	{
		ui->create_battle();
	}
	else if (id == "start_battle")
	{
		ui->start_battle();
	}
	else if (id == "join_battle")
	{
		int battle_id = std::stoi(inputs["battle_id"]->get_text());
		std::string passwd = inputs["battle_password"]->get_text();
		ui->join_battle(battle_id, passwd);
	}

	Singleton<GL_Context>().release();
}

void MainMenu::keyboard_callback(BYTE key, Keyboard::Key_action act)
{
	State initial_state = state;

	for (auto& kv : buttons)
	{
		kv.second->keyboard_callback(key, act);

		// a state change deletes all the buttons
		// so the loop must be broken
		if (state != initial_state)
			break;
	}

	if (state == initial_state)
	{
		for (auto& kv : inputs)
		{
			kv.second->key_handler(key, act);

			// a state change deletes all the inputs
			// so the loop must be broken
			if (state != initial_state)
				break;
		}
	}
}
