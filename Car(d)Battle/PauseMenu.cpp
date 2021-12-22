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
	glDisable(GL_DEPTH_TEST);

	blur.draw();
	dimmer.draw();

	for (auto& kv : buttons)
		kv.second->draw();

	for (auto& kv : texts)
		kv.second->draw();

	glEnable(GL_DEPTH_TEST);
}

void PauseMenu::set_ui(UI_Interface* ui)
{
	this->ui = ui;
}

void PauseMenu::set_screen_size(const glm::ivec2 size)
{
	const glm::vec2 ref_size(640, 480);
	const glm::vec2 ref_resolution(1280, 720);

	// case 1 - increase the menu size on larger screens:
	glm::vec2 scale(glm::vec2(screen_size.x, screen_size.y) / ref_resolution);
	scale = glm::vec2(std::min(scale.x, scale.y), std::min(scale.x, scale.y));
	glm::vec2 menu_size = ref_size * scale;

	// case 2 - don't make the menu size too small on smaller screens:
	if (menu_size.x < ref_size.x || menu_size.y < ref_size.y)
	{
		menu_size = ref_size;
	}

	// case 3 - match the menu size with a really small resolution:
	if (menu_size.x > screen_size.x)
	{
		menu_size.x = static_cast<float>(screen_size.x);
		menu_size.y = menu_size.x * ref_size.y / ref_size.x;
	}

	if (menu_size.y > screen_size.y)
	{
		menu_size.y = static_cast<float>(screen_size.y);
		menu_size.x = menu_size.y * ref_size.x / ref_size.y;
	}

	// recalculate scale:
	scale = menu_size / ref_size;
	const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1));

	// calculate origin:
	const glm::vec2 origin(
		(screen_size.x - menu_size.x) / 2,
		(screen_size.y - menu_size.y) / 2
	);
	const glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), glm::vec3(origin, 0));

	// create projection matrix:
	const glm::mat4 menu_ortho = glm::ortho(
		0.0f, static_cast<float>(screen_size.x),
		0.0f, static_cast<float>(screen_size.y)
	) * translate_mat * scale_mat;

	for (auto& kv : buttons)
	{
		kv.second->set_projection(menu_ortho);
		kv.second->set_screen_size(screen_size);
	}

	for (auto& kv : texts)
	{
		kv.second->set_mvp(menu_ortho);
	}

	dimmer.set_mvp(menu_ortho);
	dimmer.set_size(ref_size);
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
