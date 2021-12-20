#include "PauseMenu.h"

PauseMenu::PauseMenu()
	:screen_size(1, 1), ref_size(640, 480), ref_resolution(1280, 720)
{
	ui = nullptr;
	std::unique_ptr<Button> button_ptr = std::make_unique<Button>();
	buttons["leave"] = std::move(button_ptr);
	configure(buttons["leave"]);
	buttons["leave"]->set_pos({ 220, 60 });
	buttons["leave"]->set_text("Leave battle");
	buttons["leave"]->set_id("button_leave");

	buttons["resume"] = std::move(button_ptr);
	configure(buttons["resume"]);
	buttons["resume"]->set_pos({ 220, 260 });
	buttons["resume"]->set_text("Resume");
	buttons["resume"]->set_id("button_resume");

	buttons["options"] = std::move(button_ptr);
	configure(buttons["options"]);
	buttons["options"]->set_pos({ 220, 160 });
	buttons["options"]->set_text("Options...");
	buttons["options"]->set_id("button_options");

	std::unique_ptr<Text> text_ptr = std::make_unique<Text>();
	texts["title"] = std::move(text_ptr);
	configure(texts["title"]);
	texts["title"]->set_text("Pause Menu");
	// TODO: Extend the Text class with a "set_pos" member function
	// texts["opponents_num"]->set_pos({ 150, 30 });
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::configure(std::unique_ptr<Button>& button)
{
	button->set_size({ 200, 40 });
	button->set_texture("textures\\button.bmp");
	button->set_font("arial.ttf");
	button->set_projection(mvp);
	button->set_screen_size(screen_size);
	std::function<void(const std::string&)> fp =
		std::bind(&PauseMenu::button_callback, this, std::_Ph<1>());
	button->set_press_function(fp);
}

void PauseMenu::configure(std::unique_ptr<Text>& text)
{
	text->set_font("arial.ttf");
	text->set_color(glm::vec4(0, 0, 0, 1));
	text->set_mvp(mvp);
}

void PauseMenu::draw()
{
	blur.draw();
	dimmer.draw();

	for (auto& kv : buttons)
		kv.second->draw();

	for (auto& kv : texts)
		kv.second->draw();
}

void PauseMenu::set_ui(UI_Interface* ui)
{
	this->ui = ui;
}

void PauseMenu::set_screen_size(const glm::ivec2 size)
{

}

void PauseMenu::set_cursor_pos(std::pair<float, float> cursor_pos)
{

}

void PauseMenu::button_callback(const std::string& id)
{

}

void PauseMenu::keyboard_callback(BYTE key, Keyboard::Key_action act)
{

}
