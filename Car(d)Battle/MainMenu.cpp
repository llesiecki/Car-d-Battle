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

	// inputs configuration:

	std::vector<std::string> input_names = {
	"login", "password", "battle_id"
	};

	for (std::string name : input_names)
	{
		std::unique_ptr<TextInput> ptr = std::make_unique<TextInput>();
		inputs[name] = std::move(ptr);
		inputs[name]->set_font("arial.ttf");
		inputs[name]->set_cursor_pointer(cursor_pos);
		inputs[name]->set_size({ 200, 30 });
		inputs[name]->set_id(name);
		inputs[name]->set_projection(mvp);
	}

	inputs["login"]->set_pos({ 30, 30 });
	inputs["password"]->set_pos({ 260, 30 });
	inputs["battle_id"]->set_pos({ 30, 30 });

	inputs["login"]->set_text("Nickname");
	inputs["password"]->set_text("Password");
	inputs["battle_id"]->set_text("Battle ID");

	// buttons configuration:

	std::vector<std::string> button_names = {
		"login", "singleplayer", "multiplayer", "1_opponent", "2_opponents",
		"3_opponents", "create_battle", "start_battle", "join_battle",
		"leave_battle"
	};

	for (std::string& name : button_names)
	{
		std::unique_ptr<Button> ptr = std::make_unique<Button>();
		buttons[name] = std::move(ptr);
		buttons[name]->set_size({ 200, 40 });
		buttons[name]->set_texture("textures\\button.bmp");
		buttons[name]->set_font("arial.ttf");
		buttons[name]->set_id(name);
		buttons[name]->set_keyboard(kb);
		buttons[name]->set_cursor_pointer(cursor_pos);
		buttons[name]->set_projection(mvp);
		std::function<void(const std::string&)> fp =
			std::bind(&MainMenu::button_callback, this, std::_Ph<1>());
		buttons[name]->set_press_function(fp);
	}

	buttons["login"]->set_text("Login");
	buttons["singleplayer"]->set_text("Singleplayer");
	buttons["multiplayer"]->set_text("Multiplayer");
	buttons["1_opponent"]->set_text("1");
	buttons["2_opponents"]->set_text("2");
	buttons["3_opponents"]->set_text("3");
	buttons["create_battle"]->set_text("Create");
	buttons["start_battle"]->set_text("Start");
	buttons["join_battle"]->set_text("Join");
	buttons["leave_battle"]->set_text("Leave");
}

MainMenu::~MainMenu()
{
	if (game != nullptr)
		delete game;
}

void MainMenu::draw()
{
	switch (state)
	{
	case MainMenu::State::login:
		inputs["login"].draw();
		inputs["password"].draw();
		buttons["password"].draw();
		break;
	case MainMenu::State::choose_mode:
		buttons["singleplayer"].draw();
		buttons["multiplayer"].draw();
		break;
	case MainMenu::State::singleplayer:
		buttons["1_opponent"].draw();
		buttons["2_opponents"].draw();
		buttons["3_opponents"].draw();
		break;
	case MainMenu::State::multiplayer:
		buttons["create_battle"].draw();
		buttons["launch_battle"].draw();
		buttons["join_battle"].draw();
		buttons["leave_battle"].draw();
		break;
	default:
		break;
	}
}

void MainMenu::set_ui(UI_Interface* ui)
{
	this->ui = ui;
}

void MainMenu::set_screen_size(glm::ivec2 size)
{
	screen_size = size;
}

void MainMenu::button_callback(const std::string&)
{
}
