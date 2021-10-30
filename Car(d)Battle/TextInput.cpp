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
			content.pop_back();
			last_input = std::chrono::system_clock::now();
		}

		if (isalnum(key) && act == Keyboard::Key_action::on_press)
		{
			content += key;
			last_input = std::chrono::system_clock::now();
		}
	}
}
