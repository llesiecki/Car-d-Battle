#include "TextInput.h"

TextInput::TextInput()
	:texture(nullptr), scale(),
	pos(), size(), text(), shader(), transform(), proj(),
	kb(nullptr), cursor_pos(nullptr), scaled_size(), translate()
{
	active = false;
	kill_threads = false;
	draw_caret = false;
	last_input = std::chrono::system_clock::now();

	GLfloat vertices_data[] = {	// pos.x, pos.y, tex.x, tex.y
		// button rectangle with the bottom part of the texture,
		// tex coords can be shifted with a uniform
		0.0f, 1.0f,	0.0f, 0.25f,	// upper left
		0.0f, 0.0f,	0.0f, 0.0f,		// lower left
		1.0f, 0.0f,	1.0f, 0.0f,		// lower right
		1.0f, 1.0f, 1.0f, 0.25f,	// upper right
	};

	GLubyte indices[] = {
		// drawing the button rectangle:
		0, 1, 2,	// first triangle
		2, 3, 0,	// second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data),
		vertices_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

	// attrib 0 - vertex coords:
	glVertexAttribPointer(
		0, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		static_cast<void*>(0) // offset
	);
	glEnableVertexAttribArray(0);

	// attrib 1 - tex coords:
	glVertexAttribPointer(
		1, // attrib num
		2, // qty of buffer items
		GL_FLOAT, // type
		GL_FALSE, // normalize?
		4 * sizeof(GLfloat), // stride
		reinterpret_cast<void*>(2 * sizeof(GLfloat)) // offset
	);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.load("shaders\\text_input_vert.glsl", GL_VERTEX_SHADER);
	shader.load("shaders\\text_input_frag.glsl", GL_FRAGMENT_SHADER);
	shader.link();

	caret_timer = std::thread(&TextInput::caret_function, this);
}

TextInput::~TextInput()
{
	if (texture)
		delete texture;
}

void TextInput::caret_function()
{ 
	bool phase = false;

	while (!kill_threads)
	{
		if (active)
		{
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->get_id());
	shader.enable();
	shader.set("tex_id", 0);
	shader.set("transform", transform);
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, static_cast<void*>(0));

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
			std::bind(&TextInput::keyboard_callback, this, std::_Ph<1>(), std::_Ph<2>());
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

void TextInput::set_texture(const std::string& path)
{
	if (texture)
		delete texture;
	texture = new Texture(path);
	texture->load();
	texture->bind();
}
