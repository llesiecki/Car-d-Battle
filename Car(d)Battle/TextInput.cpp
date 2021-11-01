#include "TextInput.h"

TextInput::TextInput()
	:scale(), pos(), size(), text(), transform(), proj(),
	kb(nullptr), cursor_pos(nullptr), scaled_size(), translate()
{
	active = false;
	kill_threads = false;
	draw_caret = false;
	last_input = std::chrono::system_clock::now();
	caret_timer = std::thread(&TextInput::caret_function, this);
}

TextInput::~TextInput()
{

}

void TextInput::caret_function()
{ 
	bool phase = false;

	while (!kill_threads)
	{
		if (active)
		{
			using namespace std::literals::chrono_literals;

			if (std::chrono::system_clock::now() - last_input > 500ms)
			{
				thread_sleep(kill_threads, 500ms);
				if (std::chrono::system_clock::now() - last_input > 500ms)
				{
					phase ^= true;
					draw_caret = phase;
				}
				else
				{
					draw_caret = true;
				}
			}
			else
			{
				draw_caret = true;
			}
		}
		else
		{
			draw_caret = false;
		}
	}
}

void TextInput::racalculate_transform()
{
	scaled_size = glm::ivec2(size.x * scale.x, size.y * scale.y);

	transform =
		proj
		* translate
		* glm::scale(glm::mat4(1.0f), { scaled_size.x, scaled_size.y, 1 });

	float fill_y = (scaled_size.y * 0.8f) / FONT_SIZE;

	glm::mat4 text_scale = glm::scale(glm::mat4(1.0f), {
		fill_y,
		fill_y,
		1 });

	glm::mat4 text_center = glm::translate(glm::mat4(1.0f), {
		0,
		(scaled_size.y - FONT_SIZE * fill_y) / 2 + text.get_descent() * fill_y,
		0 });

	text.set_mvp(proj * translate * text_center * text_scale);
}

void TextInput::draw()
{
	glDisable(GL_DEPTH_TEST);

	dimm_center.draw();
	dimm_left.draw();
	dimm_down.draw();
	dimm_right.draw();
	dimm_up.draw();

	text.draw();

	glEnable(GL_DEPTH_TEST);
}

bool TextInput::is_hovered(const glm::ivec2& cursor) const
{
	return
		cursor.x >= pos.x &&
		cursor.x < pos.x + scaled_size.x &&
		cursor.y > pos.y &&
		cursor.y <= pos.y + scaled_size.y;
}

void TextInput::set_projection(const glm::mat4& projection)
{
	proj = projection;
	racalculate_transform();
}

void TextInput::set_pos(const glm::ivec2& pos)
{
	this->pos = pos;
	translate = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0 });
	racalculate_transform();
}

void TextInput::set_size(const glm::ivec2& size)
{
	this->size = size;

	border_width = std::min(size.x, size.y) * 0.05f;

	dimm_left.set_size({ border_width, size.y });
	dimm_right.set_size({ border_width, size.y });
	dimm_up.set_size({ size.x - 2 * border_width, border_width });
	dimm_down.set_size({ size.x - 2 * border_width, border_width });
	dimm_center.set_size(size);
	racalculate_transform();
}

void TextInput::set_scale(const glm::vec2& scale)
{
	this->scale = scale;
	racalculate_transform();
}

void TextInput::set_text(const std::string& text)
{
	this->text.set_text(text);
	racalculate_transform();
}

void TextInput::set_font(const std::string& font)
{
	this->text.set_font(font);
	racalculate_transform();
}

void TextInput::set_id(const std::string& id)
{
	this->id = id;
}

void TextInput::set_enter_function(std::function<void(const std::string&)> function)
{
	enter_function = function;
}

void TextInput::set_keyboard(Keyboard* keyboard)
{
	kb = keyboard;
	if (kb)
	{
		std::function<void(BYTE, Keyboard::Key_action)> fp =
			std::bind(
				&TextInput::keyboard_callback,
				this,
				std::_Ph<1>(), std::_Ph<2>()
			);
		kb->observe_key(VK_LBUTTON, fp);
	}
}

std::string TextInput::get_text()
{
	return content;
}

void TextInput::set_cursor_pointer(std::pair<float, float>* cursor_pos)
{
	this->cursor_pos = cursor_pos;
}

void TextInput::keyboard_callback(BYTE key, Keyboard::Key_action act)
{
	if (key == VK_LBUTTON)
	{
		if (act == Keyboard::Key_action::on_press)
		{
			if (cursor_pos)
				if (is_hovered({ cursor_pos->first, cursor_pos->second }))
				{
					active = true;
					return;
				}
			active = false;
		}
	}

	if (active)
	{
		if (key == VK_RETURN)
		{
			if (act == Keyboard::Key_action::on_release)
				enter_function(id);
		}

		if (key == VK_BACK && act == Keyboard::Key_action::on_press)
		{
			if (!content.empty())
				content.pop_back();
			last_input = std::chrono::system_clock::now();
		}

		if (isalnum(key) && act == Keyboard::Key_action::on_press)
		{
			content += key;
			text.set_text(content);

			if (text.get_width() > size.x)
			{
				content.pop_back();
				text.set_text(content);
			}

			last_input = std::chrono::system_clock::now();
		}
	}
}
