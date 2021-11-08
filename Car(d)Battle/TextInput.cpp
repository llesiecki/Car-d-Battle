#include "TextInput.h"

TextInput::TextInput()
	:scale(), pos(), size(), text(), transform(), proj(),
	cursor_pos(nullptr), scaled_size(), translate()
{
	active = false;
	kill_threads = false;
	draw_caret = false;
	last_input = std::chrono::system_clock::now();
	caret_timer = std::thread(&TextInput::caret_function, this);
}

TextInput::~TextInput()
{
	kill_threads = true;
	caret_timer.join();
}

void TextInput::caret_function()
{
	bool phase = false;

	while (!kill_threads)
	{
		using namespace std::literals::chrono_literals;

		if (active)
		{
			if (std::chrono::system_clock::now() - last_input > 500ms)
			{
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

		if (thread_sleep(kill_threads, 500ms))
		{
			break;
		}
	}
}

void TextInput::racalculate_transform()
{
	scaled_size = glm::ivec2(size.x * scale.x, size.y * scale.y);

	transform =
		proj
		* translate
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1 });

	glm::mat4 dimm_mvp = transform;

	dimm_center.set_mvp(dimm_mvp);
	dimm_left.set_mvp(dimm_mvp);

	dimm_mvp = glm::translate(transform, { border_width, 0, 0 });

	dimm_down.set_mvp(dimm_mvp);

	dimm_mvp = glm::translate(transform, { size.x - border_width, 0, 0 });

	dimm_right.set_mvp(dimm_mvp);

	dimm_mvp = glm::translate(transform, { border_width, size.y - border_width, 0 });

	dimm_up.set_mvp(dimm_mvp);

	float fill_y = (scaled_size.y * 0.8f) / FONT_SIZE;

	glm::mat4 text_scale = glm::scale(glm::mat4(1.0f), {
		fill_y,
		fill_y,
		1 });

	glm::mat4 text_pos = glm::translate(glm::mat4(1.0f), {
		FONT_SIZE * 0.2f,
		(scaled_size.y - FONT_SIZE * fill_y) / 2 + text.get_descent() * fill_y,
		0 });

	text.set_mvp(proj * translate * text_pos * text_scale);
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

std::string TextInput::get_text()
{
	return content;
}

void TextInput::set_cursor_pointer(std::pair<float, float>* cursor_pos)
{
	this->cursor_pos = cursor_pos;
}

void TextInput::key_handler(BYTE key, Keyboard::Key_action act)
{
	if(active)
		last_input = std::chrono::system_clock::now();

	if (key == VK_RETURN && active)
	{
		if (act == Keyboard::Key_action::on_release)
			enter_function(id);
	}

	if (act != Keyboard::Key_action::on_press)
		return;

	const bool shift_pressed =
		Singleton<Keyboard>().get_key_state(VK_SHIFT) ||
		Singleton<Keyboard>().get_key_state(VK_LSHIFT) ||
		Singleton<Keyboard>().get_key_state(VK_RSHIFT);

	if (key == VK_LBUTTON)
	{
		if (cursor_pos)
			if (is_hovered({ cursor_pos->first, cursor_pos->second }))
			{
				active = true;
				return;
			}
		active = false;
		return;
	}

	if (!active)
		return;
	
	if (key == VK_BACK && !content.empty())
		content.pop_back();

	char printable_char =
		Singleton<Keyboard>().key_to_char(key, shift_pressed);

	if (printable_char != '\0')
		content += printable_char;
		
	text.set_text(content);

	while (text.get_width() > size.x - 2 * (border_width + FONT_SIZE * 0.2f))
	{
		content.pop_back();
		text.set_text(content);
	}
}
