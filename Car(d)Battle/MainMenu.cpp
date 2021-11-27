#include "MainMenu.h"

MainMenu::MainMenu(
	Keyboard* kb,
	std::pair<float, float>* cursor_pos,
	const glm::mat4& mvp
)
	:screen_size(1, 1), kb(kb), cursor_pos(cursor_pos), mvp(mvp)
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
	switch (state)
	{
	case MainMenu::State::login:

		if (new_state == State::login)
		{
			state = State::login;

			buttons.clear();
			inputs.clear();
			texts.clear();

			std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
			buttons["login"] = std::move(button_ptr);
			configure(buttons["login"]);
			buttons["login"]->set_pos({ 480, 30 });
			buttons["login"]->set_text("Login");

			std::unique_ptr<TextInput> input_ptr = std::make_unique<TextInput>();
			inputs["password"] = std::move(input_ptr);
			configure(inputs["password"]);
			inputs["password"]->set_pos({ 260, 30 });
			inputs["password"]->set_text("Password");

			input_ptr = std::make_unique<TextInput>();
			inputs["login"] = std::move(input_ptr);
			configure(inputs["login"]);
			inputs["login"]->set_pos({ 30, 30 });
			inputs["login"]->set_text("Nickname");
		}

		break;

	case MainMenu::State::choose_mode:

		if (new_state == State::login)
		{
			state = State::login;

			buttons.clear();
			inputs.clear();
			texts.clear();

			std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
			buttons["login"] = std::move(button_ptr);
			configure(buttons["login"]);
			buttons["login"]->set_pos({ 480, 30 });
			buttons["login"]->set_text("Login");

			std::unique_ptr<TextInput> input_ptr = std::make_unique<TextInput>();
			inputs["password"] = std::move(input_ptr);
			configure(inputs["password"]);
			inputs["password"]->set_pos({ 260, 30 });
			inputs["password"]->set_text("Password");

			input_ptr = std::make_unique<TextInput>();
			inputs["login"] = std::move(input_ptr);
			configure(inputs["login"]);
			inputs["login"]->set_pos({ 30, 30 });
			inputs["login"]->set_text("Nickname");
		}

		if (new_state == State::singleplayer)
		{
			state = State::singleplayer;

			buttons.clear();
			inputs.clear();
			texts.clear();

			std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
			buttons["1_opponent"] = std::move(button_ptr);
			configure(buttons["1_opponent"]);
			buttons["1_opponent"]->set_pos({ 30, 30 });
			buttons["1_opponent"]->set_size({ 30, 30 });
			buttons["1_opponent"]->set_text("1");

			button_ptr = std::make_unique<Button>();
			buttons["2_opponents"] = std::move(button_ptr);
			configure(buttons["2_opponents"]);
			buttons["2_opponents"]->set_pos({ 60, 30 });
			buttons["2_opponents"]->set_size({ 30, 30 });
			buttons["2_opponents"]->set_text("2");

			button_ptr = std::make_unique<Button>();
			buttons["3_opponents"] = std::move(button_ptr);
			configure(buttons["3_opponents"]);
			buttons["3_opponents"]->set_pos({ 90, 30 });
			buttons["3_opponents"]->set_size({ 30, 30 });
			buttons["3_opponents"]->set_text("3");

			std::unique_ptr<Text> text_ptr = std::make_unique<Text>();
			texts["opponents_num"] = std::move(text_ptr);
			configure(texts["opponents_num"]);
			texts["opponents_num"]->set_text("Choose nuber of opponents:");
			// TODO: Extend the Text class with a "set_pos" member function
			//texts["opponents_num"]->set_pos({ 150, 30 });
		}

		if (new_state == State::multiplayer)
		{
			state = State::multiplayer;

			buttons.clear();
			inputs.clear();
			texts.clear();
			
			std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
			buttons["create_battle"] = std::move(button_ptr);
			configure(buttons["create_battle"]);
			buttons["create_battle"]->set_pos({ 30, 30 });
			buttons["create_battle"]->set_text("Create Battle");

			button_ptr = std::make_unique<Button>();
			buttons["start_battle"] = std::move(button_ptr);
			configure(buttons["start_battle"]);
			buttons["start_battle"]->set_pos({ 60, 30 });
			buttons["start_battle"]->set_text("Start Battle");

			button_ptr = std::make_unique<Button>();
			buttons["join_battle"] = std::move(button_ptr);
			configure(buttons["join_battle"]);
			buttons["join_battle"]->set_pos({ 90, 30 });
			buttons["join_battle"]->set_text("Join Battle");

			button_ptr = std::make_unique<Button>();
			buttons["leave_battle"] = std::move(button_ptr);
			configure(buttons["leave_battle"]);
			buttons["leave_battle"]->set_pos({ 90, 30 });
			buttons["leave_battle"]->set_text("Leave Battle");

			std::unique_ptr<TextInput> input_ptr = std::make_unique<TextInput>();
			inputs["battle_id"] = std::move(input_ptr);
			configure(inputs["battle_id"]);
			inputs["battle_id"]->set_pos({ 260, 30 });
			inputs["battle_id"]->set_text("Battle ID");
		}

		break;
	case MainMenu::State::singleplayer:

		if (new_state == State::choose_mode)
		{
			state = State::choose_mode;
		}

		break;
	case MainMenu::State::multiplayer:

		if (new_state == State::choose_mode)
		{
			state = State::choose_mode;
		}

		break;
	default:
		break;
	}
}

void MainMenu::configure(std::unique_ptr<Button>& button)
{
	button->set_size({ 200, 40 });
	button->set_texture("textures\\button.bmp");
	button->set_font("arial.ttf");
	button->set_keyboard(kb);
	button->set_cursor_pointer(cursor_pos);
	button->set_projection(mvp);
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
	input->set_cursor_pointer(cursor_pos);
	input->set_size({ 200, 30 });
	input->set_projection(mvp);
}

void MainMenu::draw()
{
	if (game != nullptr)
		game->draw();
	blur.draw();
	dimmer.draw();

	switch (state)
	{
	case MainMenu::State::login:
		inputs["login"]->draw();
		inputs["password"]->draw();
		buttons["login"]->draw();
		buttons["login"]->update();
		break;
	case MainMenu::State::choose_mode:
		buttons["singleplayer"]->draw();
		buttons["multiplayer"]->draw();
		break;
	case MainMenu::State::singleplayer:
		buttons["1_opponent"]->draw();
		buttons["2_opponents"]->draw();
		buttons["3_opponents"]->draw();
		break;
	case MainMenu::State::multiplayer:
		buttons["create_battle"]->draw();
		buttons["start_battle"]->draw();
		buttons["join_battle"]->draw();
		buttons["leave_battle"]->draw();
		inputs["battle_id"]->draw();
		break;
	default:
		break;
	}
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

void MainMenu::button_callback(const std::string& id)
{
	if (id == "singleplayer")
	{
		change_state(State::singleplayer);
	}
	else if (id == "multiplayer")
	{
		change_state(State::multiplayer);
	}
	else if (id == "button_login")
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
}
